#pragma once

void Vector_Compare()
{
	/*int n = 14999999;
	cout << "std::vector" << endl;
	{
		Ajy::StopWatch sw;
		std::vector<int> m;
		for (int i = 0; i < n; ++i)
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
		while (!m.empty())
		{
			m.pop_back();
		}
		cout << "time cost for erase:   " << sw() << endl;
		cout << m.size() << endl;
	}
	cout << "Ajy::Vector" << endl;
	{
		Ajy::StopWatch sw;
		Ajy::Vector<int> m;
		for (int i = 0; i < n; ++i)
		{
			m.Push(i);
		}
		cout << "time cost for insert:   " << sw() << endl;
		cout << m.DataLen() << endl;
		long long c = 0;
		sw.Reset();
		for (size_t i = 0; i < m.DataLen();++i)
		{
			c += m[i];
		}
		cout << "time cost for traverse:   " << sw() << endl;
		cout << c << endl;
		sw.Reset();
		while (!m.Empty())
		{
			m.Pop();
		}
		cout << "time cost for erase:   " << sw() << endl;
		cout << m.DataLen() << endl;
	}*/

	int n = 14999999;
	cout << "std::vector" << endl;
	{
		Ajy::StopWatch sw;
		std::vector<String> m;
		for (int i = 0; i < n; ++i)
		{
			m.push_back(String::MakeString(i));
		}
		cout << "time cost for insert:   " << sw() << endl;
		cout << m.size() << endl;
		long long c = 0;
		sw.Reset();
		for (auto it = m.begin(); it != m.end(); ++it)
		{
			c += 1;
		}
		cout << "time cost for traverse:   " << sw() << endl;
		cout << c << endl;
		sw.Reset();
		while (!m.empty())
		{
			m.pop_back();
		}
		cout << "time cost for erase:   " << sw() << endl;
		cout << m.size() << endl;
	}
	cout << "Ajy::Vector" << endl;
	{
		Ajy::StopWatch sw;
		Ajy::Vector<String> m;
		for (int i = 0; i < n; ++i)
		{
			m.Push(String::MakeString(i));
		}
		cout << "time cost for insert:   " << sw() << endl;
		cout << m.DataLen() << endl;
		long long c = 0;
		sw.Reset();
		for (size_t i = 0; i < m.DataLen(); ++i)
		{
			c += 1;
		}
		cout << "time cost for traverse:   " << sw() << endl;
		cout << c << endl;
		sw.Reset();
		while (!m.Empty())
		{
			m.Pop();
		}
		cout << "time cost for erase:   " << sw() << endl;
		cout << m.DataLen() << endl;
	}
}
