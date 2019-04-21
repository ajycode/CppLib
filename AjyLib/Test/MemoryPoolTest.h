#pragma once

void MemoryPool_Test()
{
	int n = 9999999;
	/*{
		StopWatch sw;
		for (int i = 0; i <= n; ++i)
		{
			auto str = new std::string("helloworld");
		}
		cout << sw() << endl;
	}
	{
		StopWatch sw;
		MemoryPool pool;
		pool.Init<std::string>(n+1);
		for (int i = 0; i <= n; ++i)
		{
			auto str = (std::string*)pool.Alloc();
			new (&str) std::string("helloworld");
		}
		cout << sw() << endl;
	}*/
	{
		StopWatch sw;
		for (int i = 0; i <= n; ++i)
		{
			auto o = new int(1);
		}
		cout << sw() << endl;
	}
	{
		StopWatch sw;
		MemoryPool pool;
		pool.Init<int>(n + 1);
		for (int i = 0; i <= n; ++i)
		{
			auto o = (int*)pool.Alloc();
			new (&o) int(1);
		}
		cout << sw() << endl;
	}
}
