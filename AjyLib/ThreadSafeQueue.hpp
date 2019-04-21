#pragma once

namespace Ajy
{
	template<class T,class Alloc = std::allocator<T>>
	class ThreadSafeQueue
	{
	public:
		ThreadSafeQueue(ThreadSafeQueue const& other) noexcept
		{
			std::unique_lock<std::mutex> lock(other._mutex);
			if (lock)
			{
				_queue = other._queue;
			}
		}
		void push(T const& data) noexcept
		{
			std::unique_lock<std::mutex> lock(_mutex);
			if (lock)
			{
				_queue.push(data);
				_dispose.notify_one();
			}
		}
		void push(T&& data) noexcept
		{
			std::unique_lock<std::mutex> lock(_mutex);
			if (lock)
			{
				_queue.push(std::forward<T>(data));
				_dispose.notify_one();
			}
		}
		T pop() noexcept
		{
			std::unique_lock<std::mutex> lock(_mutex);
			if (lock)
			{
				if (_queue.empty())
					return T();
				auto result = _queue.front();
				_queue.pop();
				return result;
			}
			return T();
		}
		T wait_and_pop() noexcept
		{
			std::unique_lock<std::mutex> lock(_mutex);
			if (lock)
			{
				_dispose.wait(lock, [this] { return !_queue.empty(); });
				T value = _queue.front();
				_queue.pop();
				return value;
			}
		}
		bool try_pop(T& value) noexcept
		{
			std::unique_lock<std::mutex> lock(_mutex);
			if (lock)
			{
				if (_queue.empty())
					return false;
				value = _queue.front();
				_queue.pop();
				return true;
			}
		}
		bool empty() const noexcept
		{
			std::unique_lock<std::mutex> lock(_mutex);
			if (lock)
			{
				return _queue.empty();
			}
			return true;
		}
	private:
		std::queue<T, Alloc> _queue;
		mutable std::mutex _mutex;
		mutable std::condition_variable _dispose;
	};

}
