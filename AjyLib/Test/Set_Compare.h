#pragma once

void Set_Compare()
{
	int n = 15000000;
	cout << "std::map" << endl;
	{
		Ajy::StopWatch sw;
		std::set<int> st;
		for (int i = 0; i < n; ++i)
		{
			st.emplace(i);
		}
		cout << "time cost for insert:   " << sw() << endl;
		cout << st.size() << endl;
		long long c = 0;
		sw.Reset();
		for (auto it = st.begin(); it != st.end(); ++it)
		{
			c += *it;
		}
		cout << "time cost for traverse:   " << sw() << endl;
		cout << c << endl;
		c = 0;
		sw.Reset();
		for (int i = 0; i < n; ++i)
		{
			c += *(st.find(i));
		}
		cout << "time cost for find:   " << sw() << endl;
		cout << c << endl;
		for (int i = 0; i < n; ++i)
		{
			st.erase(i);
		}
		cout << "time cost for erase:   " << sw() << endl;
		cout << st.size() << endl;
	}
	cout << "Ajy::Map" << endl;
	{
		Ajy::StopWatch sw;
		Ajy::Set<int> st;
		for (int i = 0; i < n; ++i)
		{
			st.Insert(i);
		}
		cout << "time cost for insert:   " << sw() << endl;
		cout << st.Size() << endl;
		long long c = 0;
		sw.Reset();
		for (auto it = st.Begin(); it != st.End(); ++it)
		{
			c += *it;
		}
		cout << "time cost for traverse:   " << sw() << endl;
		cout << c << endl;
		c = 0;
		sw.Reset();
		for (int i = 0; i < n; ++i)
		{
			c += *(st.Find(i));
		}
		cout << "time cost for find:   " << sw() << endl;
		cout << c << endl;
		for (int i = 0; i < n; ++i)
		{
			st.Delete(i);
		}
		cout << "time cost for erase:   " << sw() << endl;
		cout << st.Size() << endl;
	}
	cout << "Ajy::rbtree_set" << endl;
	{
		Ajy::StopWatch sw;
		Ajy::rbtree_set<int> st;
		for (int i = 0; i < n; ++i)
		{
			st.emplace(i);
		}
		cout << "time cost for insert:   " << sw() << endl;
		cout << st.size() << endl;
		long long c = 0;
		sw.Reset();
		for (auto it = st.begin(); it != st.end(); ++it)
		{
			c += *it;
		}
		cout << "time cost for traverse:   " << sw() << endl;
		cout << c << endl;
		c = 0;
		sw.Reset();
		for (int i = 0; i < n; ++i)
		{
			c += *(st.find(i));
		}
		cout << "time cost for find:   " << sw() << endl;
		cout << c << endl;
		for (int i = 0; i < n; ++i)
		{
			st.erase(i);
		}
		cout << "time cost for erase:   " << sw() << endl;
		cout << st.size() << endl;
	}
	cout << "Ajy::avltree_set" << endl;
	{
		Ajy::StopWatch sw;
		Ajy::avltree_set<int> st;
		for (int i = 0; i < n; ++i)
		{
			st.emplace(i);
		}
		cout << "time cost for insert:   " << sw() << endl;
		cout << st.size() << endl;
		long long c = 0;
		sw.Reset();
		for (auto it = st.begin(); it != st.end(); ++it)
		{
			c += *it;
		}
		cout << "time cost for traverse:   " << sw() << endl;
		cout << c << endl;
		c = 0;
		sw.Reset();
		for (int i = 0; i < n; ++i)
		{
			c += *(st.find(i));
		}
		cout << "time cost for find:   " << sw() << endl;
		cout << c << endl;
		for (int i = 0; i < n; ++i)
		{
			st.erase(i);
		}
		cout << "time cost for erase:   " << sw() << endl;
		cout << st.size() << endl;
	}
	cout << "Ajy::bpptree_set" << endl;
	{
		Ajy::StopWatch sw;
		Ajy::bpptree_set<int> st;
		for (int i = 0; i < n; ++i)
		{
			st.emplace(i);
		}
		cout << "time cost for insert:   " << sw() << endl;
		cout << st.size() << endl;
		long long c = 0;
		sw.Reset();
		for (auto it = st.begin(); it != st.end(); ++it)
		{
			c += *it;
		}
		cout << "time cost for traverse:   " << sw() << endl;
		cout << c << endl;
		c = 0;
		sw.Reset();
		for (int i = 0; i < n; ++i)
		{
			c += *(st.find(i));
		}
		cout << "time cost for find:   " << sw() << endl;
		cout << c << endl;
		for (int i = 0; i < n; ++i)
		{
			st.erase(i);
		}
		cout << "time cost for erase:   " << sw() << endl;
		cout << st.size() << endl;
	}
	cout << "Ajy::sbtree_set" << endl;
	{
		Ajy::StopWatch sw;
		Ajy::sbtree_multiset<int> st;
		for (int i = 0; i < n; ++i)
		{
			st.emplace(i);
		}
		cout << "time cost for insert:   " << sw() << endl;
		cout << st.size() << endl;
		long long c = 0;
		sw.Reset();
		for (auto it = st.begin(); it != st.end(); ++it)
		{
			c += *it;
		}
		cout << "time cost for traverse:   " << sw() << endl;
		cout << c << endl;
		c = 0;
		sw.Reset();
		for (int i = 0; i < n; ++i)
		{
			c += *(st.find(i));
		}
		cout << "time cost for find:   " << sw() << endl;
		cout << c << endl;
		for (int i = 0; i < n; ++i)
		{
			st.erase(i);
		}
		cout << "time cost for erase:   " << sw() << endl;
		cout << st.size() << endl;
	}
	cout << "Ajy::SkipList" << endl;
	{
		Ajy::StopWatch sw;
		Ajy::SkipList<int> st(16);
		for (int i = 0; i < n; ++i)
		{
			st.Insert(i);
		}
		cout << "time cost for insert:   " << sw() << endl;
		cout << st.Size() << endl;
		long long c = 0;
		sw.Reset();
		for (auto it = st.Begin(); it != st.End(); ++it)
		{
			c += *it;
		}
		cout << "time cost for traverse:   " << sw() << endl;
		cout << c << endl;
		c = 0;
		sw.Reset();
		for (int i = 0; i < n; ++i)
		{
			c += *(st.Find(i));
		}
		cout << "time cost for find:   " << sw() << endl;
		cout << c << endl;
		for (int i = 0; i < n; ++i)
		{
			st.Erase(i);
		}
		cout << "time cost for erase:   " << sw() << endl;
		cout << st.Size() << endl;
	}
}
