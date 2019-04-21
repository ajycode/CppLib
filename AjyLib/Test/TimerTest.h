#pragma once

void Timer_Test_1() { cout << "Timer_Test_1" << endl; }

class Timer_Test_2
{
public:
	void print() { cout << "Timer_Test_2::print" << endl; }
};

class Timer_Test_3
{
public:
	static void print() { cout << "Timer_Test_3::print" << endl; }
};

class Timer_Test_4
{
public:
	int i = 0;
	void print() {
		++i;
		cout << "Timer_Test_4::i==" << i << endl;
	}
};

void Timer_Test()
{
	Timer timer;
	timer.ScheduleOnce(10000, Timer_Test_1);
	timer.ScheduleOnce(30000, std::bind(&Timer_Test_2::print, new Timer_Test_2()));
	timer.ScheduleOnce(60000, Timer_Test_3::print);
	timer.ScheduleRepeat(1000, std::bind(&Timer_Test_4::print, Timer_Test_4()));

	std::this_thread::sleep_for(std::chrono::seconds(100));
	std::cout << "end" << std::endl;
}
