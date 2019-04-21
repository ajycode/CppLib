#pragma once

namespace Ajy
{
	// 很粗糙的实现
	// todo : 完善
	class Timer
	{
		typedef std::chrono::high_resolution_clock clock_t;

		enum class TimerType
		{
			Once,
			Repeat
		};

		struct TimerInfo
		{
			TimerType m_timerType;
			clock_t::time_point m_timePoint;
			std::size_t m_interval;
			std::function<void()> m_callback;

			template<class Func>
			TimerInfo(TimerType tt, clock_t::time_point tp, std::size_t interval, Func&& func)
				: m_timerType(tt)
				, m_timePoint(tp)
				, m_interval(interval)
				, m_callback(std::forward<Func>(func))
			{
			}
		};

		std::unique_ptr<std::thread> m_Thread;
		std::vector<TimerInfo>       m_Timers;
		std::mutex                   m_Mutex;
		std::condition_variable      m_Condition;
		bool                         m_Sort;
		bool                         m_Stop;

		void TimerLoop()
		{
			for (;;)
			{
				std::unique_lock<std::mutex>  lock(m_Mutex);

				while (!m_Stop && m_Timers.empty())
				{
					m_Condition.wait(lock);
				}

				if (m_Stop)
				{
					return;
				}

				if (m_Sort)
				{
					//Sort could be done at insert
					//but probabily this thread has time to do
					std::sort(m_Timers.begin(),
						m_Timers.end(),
						[](const TimerInfo & ti1, const TimerInfo & ti2)
					{
						return ti1.m_timePoint > ti2.m_timePoint;
					});
					m_Sort = false;
				}

				auto now = clock_t::now();
				auto expire = m_Timers.back().m_timePoint;

				if (expire > now) //can I take a nap?
				{
					auto napTime = expire - now;
					m_Condition.wait_for(lock, napTime);

					//check again
					auto expire = m_Timers.back().m_timePoint;
					auto now = clock_t::now();

					if (expire <= now)
					{
						m_Timers.back().m_callback();
						if (m_Timers.back().m_timerType == TimerType::Once)
						{
							m_Timers.pop_back();
						} 
						else
						{
							m_Timers.back().m_timePoint = now + std::chrono::milliseconds(m_Timers.back().m_interval);
							m_Sort = true;
						}
					}
				}
				else
				{
					m_Timers.back().m_callback();
					m_Timers.pop_back();
					if (m_Timers.back().m_timerType == TimerType::Once)
					{
						m_Timers.pop_back();
					}
					else
					{
						m_Timers.back().m_timePoint = now + std::chrono::milliseconds(m_Timers.back().m_interval);
					}
				}
			}
		}

	public:
		Timer() : m_Stop(false), m_Sort(false)
		{
			m_Thread.reset(new std::thread(std::bind(&Timer::TimerLoop, this)));
		}

		~Timer()
		{
			m_Stop = true;
			m_Condition.notify_all();
			m_Thread->join();
		}

		template<class Func>
		void ScheduleOnce(std::size_t ms_interval, Func&& func)
		{
			std::unique_lock<std::mutex> lock(m_Mutex);
			m_Timers.emplace_back(Timer::TimerInfo(TimerType::Once, Timer::clock_t::now() + std::chrono::milliseconds(ms_interval), ms_interval, std::forward<Func>(func)));
			m_Sort = true;
			// wake up
			m_Condition.notify_one();
		}

		template<class Func>
		void ScheduleRepeat(std::size_t ms_interval, Func&& func)
		{
			std::unique_lock<std::mutex> lock(m_Mutex);
			m_Timers.emplace_back(Timer::TimerInfo(TimerType::Repeat, Timer::clock_t::now() + std::chrono::milliseconds(ms_interval), ms_interval, std::forward<Func>(func)));
			m_Sort = true;
			// wake up
			m_Condition.notify_one();
		}
	};

}

