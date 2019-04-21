#pragma once

void SkipList_Test()
{
	/************************************************************************/
	/* Test                                                                 */
	/************************************************************************/

	SkipList<int> sl;
	std::vector<int> stdvec;
	int n = 42;
	for (int i = 0; i < n; ++i)
	{
		stdvec.push_back(i);
	}
	std::random_shuffle(stdvec.begin(), stdvec.end());
	for (auto it = stdvec.begin(); it != stdvec.end(); ++it)
	{
		sl.Insert(*it);
	}
	Cout.WriteLine(sl.Size(), "\n");
	Cout.WriteLine("-------------------------------");

	for (int i = 0; i < n; ++i)
	{
		auto it = sl.Find(i);
		if (it != sl.End())
		{
			Cout.Write(*it, "   ");
		}
	}
	Cout.WriteLine();
	Cout.WriteLine("-------------------------------");

	for (auto it = sl.Begin(); it != sl.End(); ++it)
	{
		Cout.Write(*it, "   ");
	}
	Cout.WriteLine();
	Cout.WriteLine("-------------------------------");

	for (auto it = sl.Begin(); it != sl.End();)
	{
		Cout.Write(*it, "   ");
		it = sl.Erase(it);
	}
	Cout.WriteLine("");
	Cout.WriteLine(sl.Size());
	Cout.WriteLine("-------------------------------");

	for (auto it = sl.Begin(); it != sl.End(); ++it)
	{
		Cout.Write(*it, "   ");
	}
	Cout.WriteLine("");
	Cout.WriteLine(sl.Size());
	Cout.WriteLine("-------------------------------");

	for (int i = 0; i < n; ++i)
	{
		sl.Insert(i);
	}
	sl.Insert(-1);
	for (auto it = sl.Begin(); it != sl.End(); ++it)
	{
		Cout.Write(*it, "   ");
	}
	Cout.WriteLine("");
	Cout.WriteLine(sl.Size());
	Cout.WriteLine("-------------------------------");

	SkipList<int> sl1(sl);
	for (auto it = sl1.Begin(); it != sl1.End(); ++it)
	{
		Cout.Write(*it, "   ");
	}
	Cout.WriteLine("");
	Cout.WriteLine(sl1.Size());
	Cout.WriteLine("-------------------------------");

	sl.Clear();
	for (auto it = sl.Begin(); it != sl.End(); ++it)
	{
		Cout.Write(*it, "   ");
	}
	Cout.WriteLine("");
	Cout.WriteLine(sl.Size());
	Cout.WriteLine("-------------------------------");

	SkipList<int> sl2(std::move(sl1));
	for (auto it = sl2.Begin(); it != sl2.End(); ++it)
	{
		Cout.Write(*it, "   ");
	}
	Cout.WriteLine("");
	Cout.WriteLine(sl2.Size());
	for (auto it = sl1.Begin(); it != sl1.End(); ++it)
	{
		Cout.Write(*it, "   ");
	}
	Cout.WriteLine("");
	Cout.WriteLine(sl1.Size());
	Cout.WriteLine("-------------------------------");

	sl1 = std::move(sl2);
	for (auto it = sl2.Begin(); it != sl2.End(); ++it)
	{
		Cout.Write(*it, "   ");
	}
	Cout.WriteLine("");
	Cout.WriteLine(sl2.Size());
	for (auto it = sl1.Begin(); it != sl1.End(); ++it)
	{
		Cout.Write(*it, "   ");
	}
	Cout.WriteLine("");
	Cout.WriteLine(sl1.Size());
	Cout.WriteLine("-------------------------------");

	sl = sl1;
	for (auto it = sl1.Begin(); it != sl1.End(); ++it)
	{
		Cout.Write(*it, "   ");
	}
	Cout.WriteLine("");
	Cout.WriteLine(sl1.Size());
	for (auto it = sl.Begin(); it != sl.End(); ++it)
	{
		Cout.Write(*it, "   ");
	}
	Cout.WriteLine("");
	Cout.WriteLine(sl.Size());
	Cout.WriteLine("-------------------------------");
	SkipList<int>::Iterator it = sl.Find(3);
	if (it != sl.End())
	{
		Cout.WriteLine(*it);
	}
	SkipList<int>::Iterator it1 = sl.Find(300);
	if (it1 != sl.End())
	{
		Cout.WriteLine(*it1);
	}



	/************************************************************************/
	/* Compare with RBTree                                                  */
	/************************************************************************/

	/*std::vector<int> stdvec;
	int n = 15000000;
	for (int i = 0; i < n; ++i)
	{
		stdvec.push_back(i);
	}
	std::random_shuffle(stdvec.begin(), stdvec.end());
	cout << "std::set<int>" << endl;
	cout << "{" << endl;
	{
		std::set<int> st;
		StopWatch sw;
		for (auto it = stdvec.begin(); it != stdvec.end(); ++it)
		{
			st.emplace(*it);
		}
		cout << "time(ms) cost for insert:   " << sw() << endl;
		cout << st.size() << endl;
		long long c = 0;
		sw.Reset();
		for (auto it = st.begin(); it != st.end(); ++it)
		{
			c += *it;
		}
		cout << "time(ms) cost for traverse:   " << sw() << endl;
		cout << c << endl;
		c = 0;
		sw.Reset();
		for (int i = 1; i <= n; ++i)
		{
			auto it = st.find(i);
			if (it != st.end())
			{
				c += *it;
			}
		}
		cout << "time(ms) cost for find:   " << sw() << endl;
		cout << c << endl;
		c = 0;
		sw.Reset();
		for (auto it = st.begin(); it != st.end();)
		{
			c += *it;
			it = st.erase(it);
		}
		cout << "time(ms) cost for erase:   " << sw() << endl;
		cout << c << endl;
	}
	cout << "}" << endl;
	cout << "Ajy::SkipList<int>" << endl;
	cout << "{" << endl;
	{
		Ajy::SkipList<int> sl(16);
		StopWatch sw;
		for (auto it = stdvec.begin(); it != stdvec.end(); ++it)
		{
			sl.Insert(*it);
		}
		cout << "time(ms) cost for insert:   " << sw() << endl;
		cout << sl.Size() << endl;
		long long c = 0;
		sw.Reset();
		for (auto it = sl.Begin(); it != sl.End(); ++it)
		{
			c += *it;
		}
		cout << "time(ms) cost for traverse:   " << sw() << endl;
		cout << c << endl;
		sw.Reset();
		for (int i = 1; i <= n; ++i)
		{
			auto it = sl.Find(i);
			if (it != sl.End())
			{
				c += *it;
			}
		}
		cout << "time(ms) cost for find:   " << sw() << endl;
		cout << c << endl;
		c = 0;
		sw.Reset();
		for (auto it = sl.Begin(); it != sl.End();)
		{
			c += *it;
			it = sl.Erase(it);
		}
		cout << "time(ms) cost for erase:   " << sw() << endl;
		cout << c << endl;
	}
	cout << "}" << endl;*/
}

