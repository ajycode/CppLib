#pragma once

void List_Compare()
{
	//
	int n = 14999999;
	cout << "std::list" << endl;
	{
		Ajy::StopWatch sw;
		std::list<int> m;
		for (int i = 0; i <= n; ++i)
		{
			m.push_back(i);
		}
		cout << "time cost for insert:   " << sw() << endl;
		cout << m.size() << endl;
		long long c = 0;
		sw.Reset();
		for (auto it = m.begin(); it != m.end(); ++it)
		{
			c += *it;
		}
		cout << "time cost for traverse:   " << sw() << endl;
		cout << c << endl;
		sw.Reset();
		for (auto it = m.begin(); it != m.end();)
		{
			it = m.erase(it);
		}
		cout << "time cost for erase:   " << sw() << endl;
		cout << m.size() << endl;
	}
	cout << "Ajy::List" << endl;
	{
		Ajy::StopWatch sw;
		Ajy::List<int> m;
		for (int i = 0; i <= n; ++i)
		{
			m.PushBack(i);
		}
		cout << "time cost for insert:   " << sw() << endl;
		cout << m.Size() << endl;
		long long c = 0;
		sw.Reset();
		for (auto it = m.Begin(); it != m.End(); ++it)
		{
			c += *it;
		}
		cout << "time cost for traverse:   " << sw() << endl;
		cout << c << endl;
		sw.Reset();
		for (auto it = m.Begin(); it != m.End();)
		{
			it = m.Erase(it);
		}
		cout << "time cost for erase:   " << sw() << endl;
		cout << m.Size() << endl;
	}

	//
	/*int n = 14999999;
	cout << "std::list" << endl;
	{
		Ajy::StopWatch sw;
		std::list<std::string> m;
		for (int i = 0; i < n; ++i)
		{
			char str[10];
			sprintf(str, "%d", i);
			m.push_back(str);
		}
		cout << "time cost for insert:   " << sw() << endl;
		cout << m.size() << endl;
		long long c = 0;
		sw.Reset();
		for (auto it = m.begin(); it != m.end(); ++it)
		{
			int i = 0;
		}
		cout << "time cost for traverse:   " << sw() << endl;
		cout << c << endl;
		sw.Reset();
		for (auto it = m.begin(); it != m.end();)
		{
			it = m.erase(it);
		}
		cout << "time cost for erase:   " << sw() << endl;
		cout << m.size() << endl;
	}
	cout << "Ajy::List" << endl;
	{
		Ajy::StopWatch sw;
		Ajy::List<std::string> m;
		for (int i = 0; i < n; ++i)
		{
			char str[10];
			sprintf(str, "%d", i);
			m.PushBack(str);
		}
		cout << "time cost for insert:   " << sw() << endl;
		cout << m.Size() << endl;
		long long c = 0;
		sw.Reset();
		for (auto it = m.Begin(); it != m.End(); ++it)
		{
			int i = 0;
		}
		cout << "time cost for traverse:   " << sw() << endl;
		cout << c << endl;
		sw.Reset();
		for (auto it = m.Begin(); it != m.End();)
		{
			it = m.Erase(it);
		}
		cout << "time cost for erase:   " << sw() << endl;
		cout << m.Size() << endl;
	}*/
}
