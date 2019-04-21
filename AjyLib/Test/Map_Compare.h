#pragma once

void Map_Compare_1()
{
	int n = 15000000;
	cout << "std::map" << endl;
	{
		Ajy::StopWatch sw;
		std::map<int, int> mp;
		for (int i = 0; i < n; ++i)
		{
			mp.emplace(i, i);
		}
		cout << "time cost for insert:   " << sw() << endl;
		cout << mp.size() << endl;
		mp.erase(n / 2);
		long long c = 0;
		sw.Reset();
		for (auto it = mp.begin(); it != mp.end(); ++it)
		{
			c += it->second;
		}
		cout << "time cost for traverse:   " << sw() << endl;
		cout << c << endl;
		c = 0;
		sw.Reset();
		for (int i = 0; i < n; ++i)
		{
			auto it = mp.find(i);
			if (it != mp.end())
			{
				c += it->second;
			}
		}
		cout << "time cost for find:   " << sw() << endl;
		cout << c << endl;
		for (auto it = mp.begin(); it != mp.end();)
		{
			it = mp.erase(it);
		}
		cout << "time cost for erase:   " << sw() << endl;
		cout << mp.size() << endl;
	}
	cout << "Ajy::Map" << endl;
	{
		Ajy::StopWatch sw;
		Ajy::Map<int, int> mp;
		for (int i = 0; i < n; ++i)
		{
			mp.Insert(std::pair<int, int>(i, i));
		}
		cout << "time cost for insert:   " << sw() << endl;
		cout << mp.Size() << endl;
		mp.Delete(n / 2);
		long long c = 0;
		sw.Reset();
		for (auto it = mp.Begin(); it != mp.End(); ++it)
		{
			c += it->second;
		}
		cout << "time cost for traverse:   " << sw() << endl;
		cout << c << endl;
		c = 0;
		sw.Reset();
		for (int i = 0; i < n; ++i)
		{
			auto it = mp.Find(i);
			if (it != mp.End())
			{
				c += it->second;
			}
		}
		cout << "time cost for find:   " << sw() << endl;
		cout << c << endl;
		for (auto it = mp.Begin(); it != mp.End();)
		{
			it = mp.Delete(it);
		}
		cout << "time cost for erase:   " << sw() << endl;
		cout << mp.Size() << endl;
	}
	cout << "Ajy::rbtree_map" << endl;
	{
		Ajy::StopWatch sw;
		Ajy::rbtree_map<int, int> mp;
		for (int i = 0; i < n; ++i)
		{
			mp.emplace(i, i);
		}
		cout << "time cost for insert:   " << sw() << endl;
		cout << mp.size() << endl;
		mp.erase(n / 2);
		long long c = 0;
		sw.Reset();
		for (auto it = mp.begin(); it != mp.end(); ++it)
		{
			c += it->second;
		}
		cout << "time cost for traverse:   " << sw() << endl;
		cout << c << endl;
		c = 0;
		sw.Reset();
		for (int i = 0; i < n; ++i)
		{
			auto it = mp.find(i);
			if (it != mp.end())
			{
				c += it->second;
			}
		}
		cout << "time cost for find:   " << sw() << endl;
		cout << c << endl;
		for (auto it = mp.begin(); it != mp.end();)
		{
			it = mp.erase(it);
		}
		cout << "time cost for erase:   " << sw() << endl;
		cout << mp.size() << endl;
	}
	cout << "Ajy::bpptree_map" << endl;
	{
		Ajy::StopWatch sw;
		Ajy::bpptree_map<int, int> mp;
		for (int i = 0; i < n; ++i)
		{
			mp.emplace(i, i);
		}
		cout << "time cost for insert:   " << sw() << endl;
		cout << mp.size() << endl;
		mp.erase(n / 2);
		long long c = 0;
		sw.Reset();
		for (auto it = mp.begin(); it != mp.end(); ++it)
		{
			c += it->second;
		}
		cout << "time cost for traverse:   " << sw() << endl;
		cout << c << endl;
		c = 0;
		sw.Reset();
		for (int i = 0; i < n; ++i)
		{
			auto it = mp.find(i);
			if (it != mp.end())
			{
				c += it->second;
			}
		}
		cout << "time cost for find:   " << sw() << endl;
		cout << c << endl;
		for (auto it = mp.begin(); it != mp.end();)
		{
			it = mp.erase(it);
		}
		cout << "time cost for erase:   " << sw() << endl;
		cout << mp.size() << endl;
	}
	cout << "Ajy::avltree_map" << endl;
	{
		Ajy::StopWatch sw;
		Ajy::avltree_multimap<int, int> mp;
		for (int i = 0; i < n; ++i)
		{
			mp.emplace(i, i);
		}
		cout << "time cost for insert:   " << sw() << endl;
		cout << mp.size() << endl;
		mp.erase(n / 2);
		long long c = 0;
		sw.Reset();
		for (auto it = mp.begin(); it != mp.end(); ++it)
		{
			c += it->second;
		}
		cout << "time cost for traverse:   " << sw() << endl;
		cout << c << endl;
		c = 0;
		sw.Reset();
		for (int i = 0; i < n; ++i)
		{
			auto it = mp.find(i);
			if (it != mp.end())
			{
				c += it->second;
			}
		}
		cout << "time cost for find:   " << sw() << endl;
		cout << c << endl;
		for (auto it = mp.begin(); it != mp.end();)
		{
			it = mp.erase(it);
		}
		cout << "time cost for erase:   " << sw() << endl;
		cout << mp.size() << endl;
	}
	cout << "Ajy::sbtree_map" << endl;
	{
		Ajy::StopWatch sw;
		Ajy::sbtree_multimap<int, int> mp;
		for (int i = 0; i < n; ++i)
		{
			mp.emplace(i, i);
		}
		cout << "time cost for insert:   " << sw() << endl;
		cout << mp.size() << endl;
		mp.erase(n / 2);
		long long c = 0;
		sw.Reset();
		for (auto it = mp.begin(); it != mp.end(); ++it)
		{
			c += it->second;
		}
		cout << "time cost for traverse:   " << sw() << endl;
		cout << c << endl;
		c = 0;
		sw.Reset();
		for (int i = 0; i < n; ++i)
		{
			auto it = mp.find(i);
			if (it != mp.end())
			{
				c += it->second;
			}
		}
		cout << "time cost for find:   " << sw() << endl;
		cout << c << endl;
		for (auto it = mp.begin(); it != mp.end();)
		{
			it = mp.erase(it);
		}
		cout << "time cost for erase:   " << sw() << endl;
		cout << mp.size() << endl;
	}
	cout << "==============================================================" << endl;
	std::vector<int> vec;
	for (int i = 0; i < n; ++i)
	{
		vec.emplace_back(i);
	}
	std::random_shuffle(vec.begin(), vec.end());
	cout << "std::map" << endl;
	{
		Ajy::StopWatch sw;
		std::map<int, int> mp;
		for (int i = 0; i < n; ++i)
		{
			mp.emplace(vec[i], vec[i]);
		}
		cout << "time cost for insert:   " << sw() << endl;
		cout << mp.size() << endl;
		mp.erase(n / 2);
		long long c = 0;
		sw.Reset();
		for (auto it = mp.begin(); it != mp.end(); ++it)
		{
			c += it->second;
		}
		cout << "time cost for traverse:   " << sw() << endl;
		cout << c << endl;
		c = 0;
		sw.Reset();
		for (int i = 0; i < n; ++i)
		{
			auto it = mp.find(vec[i]);
			if (it != mp.end())
			{
				c += it->second;
			}
		}
		cout << "time cost for find:   " << sw() << endl;
		cout << c << endl;
		for (auto it = mp.begin(); it != mp.end();)
		{
			it = mp.erase(it);
		}
		cout << "time cost for erase:   " << sw() << endl;
		cout << mp.size() << endl;
	}
	cout << "Ajy::Map" << endl;
	{
		Ajy::StopWatch sw;
		Ajy::Map<int, int> mp;
		for (int i = 0; i < n; ++i)
		{
			mp.Insert(std::pair<int, int>(vec[i], vec[i]));
		}
		cout << "time cost for insert:   " << sw() << endl;
		cout << mp.Size() << endl;
		mp.Delete(n / 2);
		long long c = 0;
		sw.Reset();
		for (auto it = mp.Begin(); it != mp.End(); ++it)
		{
			c += it->second;
		}
		cout << "time cost for traverse:   " << sw() << endl;
		cout << c << endl;
		c = 0;
		sw.Reset();
		for (int i = 0; i < n; ++i)
		{
			auto it = mp.Find(vec[i]);
			if (it != mp.End())
			{
				c += it->second;
			}
		}
		cout << "time cost for find:   " << sw() << endl;
		cout << c << endl;
		for (auto it = mp.Begin(); it != mp.End();)
		{
			it = mp.Delete(it);
		}
		cout << "time cost for erase:   " << sw() << endl;
		cout << mp.Size() << endl;
	}
	cout << "Ajy::rbtree_map" << endl;
	{
		Ajy::StopWatch sw;
		Ajy::rbtree_map<int, int> mp;
		for (int i = 0; i < n; ++i)
		{
			mp.emplace(vec[i], vec[i]);
		}
		cout << "time cost for insert:   " << sw() << endl;
		cout << mp.size() << endl;
		mp.erase(n / 2);
		long long c = 0;
		sw.Reset();
		for (auto it = mp.begin(); it != mp.end(); ++it)
		{
			c += it->second;
		}
		cout << "time cost for traverse:   " << sw() << endl;
		cout << c << endl;
		c = 0;
		sw.Reset();
		for (int i = 0; i < n; ++i)
		{
			auto it = mp.find(vec[i]);
			if (it != mp.end())
			{
				c += it->second;
			}
		}
		cout << "time cost for find:   " << sw() << endl;
		cout << c << endl;
		for (auto it = mp.begin(); it != mp.end();)
		{
			it = mp.erase(it);
		}
		cout << "time cost for erase:   " << sw() << endl;
		cout << mp.size() << endl;
	}
	cout << "Ajy::bpptree_map" << endl;
	{
		Ajy::StopWatch sw;
		Ajy::bpptree_map<int, int> mp;
		for (int i = 0; i < n; ++i)
		{
			mp.emplace(vec[i], vec[i]);
		}
		cout << "time cost for insert:   " << sw() << endl;
		cout << mp.size() << endl;
		mp.erase(n / 2);
		long long c = 0;
		sw.Reset();
		for (auto it = mp.begin(); it != mp.end(); ++it)
		{
			c += it->second;
		}
		cout << "time cost for traverse:   " << sw() << endl;
		cout << c << endl;
		c = 0;
		sw.Reset();
		for (int i = 0; i < n; ++i)
		{
			auto it = mp.find(vec[i]);
			if (it != mp.end())
			{
				c += it->second;
			}
		}
		cout << "time cost for find:   " << sw() << endl;
		cout << c << endl;
		for (auto it = mp.begin(); it != mp.end();)
		{
			it = mp.erase(it);
		}
		cout << "time cost for erase:   " << sw() << endl;
		cout << mp.size() << endl;
	}
	cout << "Ajy::avltree_map" << endl;
	{
		Ajy::StopWatch sw;
		Ajy::avltree_map<int, int> mp;
		for (int i = 0; i < n; ++i)
		{
			mp.emplace(vec[i], vec[i]);
		}
		cout << "time cost for insert:   " << sw() << endl;
		cout << mp.size() << endl;
		mp.erase(n / 2);
		long long c = 0;
		sw.Reset();
		for (auto it = mp.begin(); it != mp.end(); ++it)
		{
			c += it->second;
		}
		cout << "time cost for traverse:   " << sw() << endl;
		cout << c << endl;
		c = 0;
		sw.Reset();
		for (int i = 0; i < n; ++i)
		{
			auto it = mp.find(vec[i]);
			if (it != mp.end())
			{
				c += it->second;
			}
		}
		cout << "time cost for find:   " << sw() << endl;
		cout << c << endl;
		for (auto it = mp.begin(); it != mp.end();)
		{
			it = mp.erase(it);
		}
		cout << "time cost for erase:   " << sw() << endl;
		cout << mp.size() << endl;
	}
	cout << "Ajy::sbtree_map" << endl;
	{
		Ajy::StopWatch sw;
		Ajy::sbtree_multimap<int, int> mp;
		for (int i = 0; i < n; ++i)
		{
			mp.emplace(vec[i], vec[i]);
		}
		cout << "time cost for insert:   " << sw() << endl;
		cout << mp.size() << endl;
		mp.erase(n / 2);
		long long c = 0;
		sw.Reset();
		for (auto it = mp.begin(); it != mp.end(); ++it)
		{
			c += it->second;
		}
		cout << "time cost for traverse:   " << sw() << endl;
		cout << c << endl;
		c = 0;
		sw.Reset();
		for (int i = 0; i < n; ++i)
		{
			auto it = mp.find(vec[i]);
			if (it != mp.end())
			{
				c += it->second;
			}
		}
		cout << "time cost for find:   " << sw() << endl;
		cout << c << endl;
		for (auto it = mp.begin(); it != mp.end();)
		{
			it = mp.erase(it);
		}
		cout << "time cost for erase:   " << sw() << endl;
		cout << mp.size() << endl;
	}
}

void Map_Compare_2()
{
	int n = 15000000;
	cout << "std::map" << endl;
	{
	Ajy::StopWatch sw;
	std::map<std::string, int> mp;
	for (int i = 0; i < n; ++i)
	{
	char buf[10];
	sprintf(buf, "%d", i);
	mp.emplace(std::string(buf), i);
	}
	cout << "time cost for insert:   " << sw() << endl;
	cout << mp.size() << endl;
	long long c = 0;
	sw.Reset();
	for (auto it = mp.begin(); it != mp.end(); ++it)
	{
	c += it->second;
	}
	cout << "time cost for traverse:   " << sw() << endl;
	cout << c << endl;
	c = 0;
	sw.Reset();
	for (int i = 0; i < n; ++i)
	{
	char buf[10];
	sprintf(buf, "%d", i);
	c += mp.find(std::string(buf))->second;
	}
	cout << "time cost for find:   " << sw() << endl;
	cout << c << endl;
	sw.Reset();
	for (auto it = mp.begin(); it != mp.end();)
	{
	it = mp.erase(it);
	}
	cout << "time cost for erase:   " << sw() << endl;
	cout << mp.size() << endl;
	}
	cout << "Ajy::Map" << endl;
	{
	Ajy::StopWatch sw;
	Ajy::Map<std::string, int> mp;
	for (int i = 0; i < n; ++i)
	{
	char buf[10];
	sprintf(buf, "%d", i);
	mp.Insert(std::pair<std::string, int>(std::string(buf), i));
	}
	cout << "time cost for insert:   " << sw() << endl;
	cout << mp.Size() << endl;
	long long c = 0;
	sw.Reset();
	for (auto it = mp.Begin(); it != mp.End(); ++it)
	{
	c += it->second;
	}
	cout << "time cost for traverse:   " << sw() << endl;
	cout << c << endl;
	c = 0;
	sw.Reset();
	for (int i = 0; i < n; ++i)
	{
	char buf[10];
	sprintf(buf, "%d", i);
	c += mp.Find(std::string(buf))->second;
	}
	cout << "time cost for find:   " << sw() << endl;
	cout << c << endl;
	sw.Reset();
	for (auto it = mp.Begin(); it != mp.End();)
	{
	it = mp.Delete(it);
	}
	cout << "time cost for erase:   " << sw() << endl;
	cout << mp.Size() << endl;
	}
	cout << "Ajy::bpptree_map" << endl;
	{
	Ajy::StopWatch sw;
	Ajy::bpptree_map<std::string, int> mp;
	for (int i = 0; i < n; ++i)
	{
	char buf[10];
	sprintf(buf, "%d", i);
	mp.emplace(std::string(buf), i);
	}
	cout << "time cost for insert:   " << sw() << endl;
	cout << mp.size() << endl;
	long long c = 0;
	sw.Reset();
	for (auto it = mp.begin(); it != mp.end(); ++it)
	{
	c += it->second;
	}
	cout << "time cost for traverse:   " << sw() << endl;
	cout << c << endl;
	c = 0;
	sw.Reset();
	for (int i = 0; i < n; ++i)
	{
	char buf[10];
	sprintf(buf, "%d", i);
	c += mp.find(std::string(buf))->second;
	}
	cout << "time cost for find:   " << sw() << endl;
	cout << c << endl;
	sw.Reset();
	for (auto it = mp.begin(); it != mp.end();)
	{
	it = mp.erase(it);
	}
	cout << "time cost for erase:   " << sw() << endl;
	cout << mp.size() << endl;
	}
	cout << "Ajy::rbtree_map" << endl;
	{
	Ajy::StopWatch sw;
	Ajy::rbtree_map<std::string, int> mp;
	for (int i = 0; i < n; ++i)
	{
	char buf[10];
	sprintf(buf, "%d", i);
	mp.emplace(std::string(buf), i);
	}
	cout << "time cost for insert:   " << sw() << endl;
	cout << mp.size() << endl;
	long long c = 0;
	sw.Reset();
	for (auto it = mp.begin(); it != mp.end(); ++it)
	{
	c += it->second;
	}
	cout << "time cost for traverse:   " << sw() << endl;
	cout << c << endl;
	c = 0;
	sw.Reset();
	for (int i = 0; i < n; ++i)
	{
	char buf[10];
	sprintf(buf, "%d", i);
	c += mp.find(std::string(buf))->second;
	}
	cout << "time cost for find:   " << sw() << endl;
	cout << c << endl;
	sw.Reset();
	for (auto it = mp.begin(); it != mp.end();)
	{
	it = mp.erase(it);
	}
	cout << "time cost for erase:   " << sw() << endl;
	cout << mp.size() << endl;
	}
	cout << "Ajy::avltree_map" << endl;
	{
	Ajy::StopWatch sw;
	Ajy::avltree_map<std::string, int> mp;
	for (int i = 0; i < n; ++i)
	{
	char buf[10];
	sprintf(buf, "%d", i);
	mp.emplace(std::string(buf), i);
	}
	cout << "time cost for insert:   " << sw() << endl;
	cout << mp.size() << endl;
	long long c = 0;
	sw.Reset();
	for (auto it = mp.begin(); it != mp.end(); ++it)
	{
	c += it->second;
	}
	cout << "time cost for traverse:   " << sw() << endl;
	cout << c << endl;
	c = 0;
	sw.Reset();
	for (int i = 0; i < n; ++i)
	{
	char buf[10];
	sprintf(buf, "%d", i);
	c += mp.find(std::string(buf))->second;
	}
	cout << "time cost for find:   " << sw() << endl;
	cout << c << endl;
	sw.Reset();
	for (auto it = mp.begin(); it != mp.end();)
	{
	it = mp.erase(it);
	}
	cout << "time cost for erase:   " << sw() << endl;
	cout << mp.size() << endl;
	}
	cout << "Ajy::sbtree_map" << endl;
	{
	Ajy::StopWatch sw;
	Ajy::sbtree_multimap<std::string, int> mp;
	for (int i = 0; i < n; ++i)
	{
	char buf[10];
	sprintf(buf, "%d", i);
	mp.emplace(std::string(buf), i);
	}
	cout << "time cost for insert:   " << sw() << endl;
	cout << mp.size() << endl;
	long long c = 0;
	sw.Reset();
	for (auto it = mp.begin(); it != mp.end(); ++it)
	{
	c += it->second;
	}
	cout << "time cost for traverse:   " << sw() << endl;
	cout << c << endl;
	c = 0;
	sw.Reset();
	for (int i = 0; i < n; ++i)
	{
	char buf[10];
	sprintf(buf, "%d", i);
	c += mp.find(std::string(buf))->second;
	}
	cout << "time cost for find:   " << sw() << endl;
	cout << c << endl;
	sw.Reset();
	for (auto it = mp.begin(); it != mp.end();)
	{
	it = mp.erase(it);
	}
	cout << "time cost for erase:   " << sw() << endl;
	cout << mp.size() << endl;
	}
}

void Map_Compare_3()
{
	int n = 15000000;
	cout << "std::map" << endl;
	{
		Ajy::StopWatch sw;
		std::map<Ajy::String, int> mp;
		for (int i = 0; i < n; ++i)
		{
			mp.emplace(String::MakeString(i), i);
		}
		cout << "time cost for insert:   " << sw() << endl;
		cout << mp.size() << endl;
		long long c = 0;
		sw.Reset();
		for (auto it = mp.begin(); it != mp.end(); ++it)
		{
			c += it->second;
		}
		cout << "time cost for traverse:   " << sw() << endl;
		cout << c << endl;
		c = 0;
		sw.Reset();
		for (int i = 0; i < n; ++i)
		{
			c += mp.find(String::MakeString(i))->second;
		}
		cout << "time cost for find:   " << sw() << endl;
		cout << c << endl;
		sw.Reset();
		for (auto it = mp.begin(); it != mp.end();)
		{
			it = mp.erase(it);
		}
		cout << "time cost for erase:   " << sw() << endl;
		cout << mp.size() << endl;
	}
	cout << "Ajy::Map" << endl;
	{
		Ajy::StopWatch sw;
		Ajy::Map<Ajy::String, int> mp;
		for (int i = 0; i < n; ++i)
		{
			mp.Insert(std::make_pair(String::MakeString(i), i));
		}
		cout << "time cost for insert:   " << sw() << endl;
		cout << mp.Size() << endl;
		long long c = 0;
		sw.Reset();
		for (auto it = mp.Begin(); it != mp.End(); ++it)
		{
			c += it->second;
		}
		cout << "time cost for traverse:   " << sw() << endl;
		cout << c << endl;
		c = 0;
		sw.Reset();
		for (int i = 0; i < n; ++i)
		{
			c += mp.Find(String::MakeString(i))->second;
		}
		cout << "time cost for find:   " << sw() << endl;
		cout << c << endl;
		sw.Reset();
		for (auto it = mp.Begin(); it != mp.End();)
		{
			it = mp.Delete(it);
		}
		cout << "time cost for erase:   " << sw() << endl;
		cout << mp.Size() << endl;
	}
	cout << "Ajy::rbtree_map" << endl;
	{
		Ajy::StopWatch sw;
		Ajy::rbtree_map<Ajy::String, int> mp;
		for (int i = 0; i < n; ++i)
		{
			mp.emplace(String::MakeString(i), i);
		}
		cout << "time cost for insert:   " << sw() << endl;
		cout << mp.size() << endl;
		long long c = 0;
		sw.Reset();
		for (auto it = mp.begin(); it != mp.end(); ++it)
		{
			c += it->second;
		}
		cout << "time cost for traverse:   " << sw() << endl;
		cout << c << endl;
		c = 0;
		sw.Reset();
		for (int i = 0; i < n; ++i)
		{
			c += mp.find(String::MakeString(i))->second;
		}
		cout << "time cost for find:   " << sw() << endl;
		cout << c << endl;
		sw.Reset();
		for (auto it = mp.begin(); it != mp.end();)
		{
			it = mp.erase(it);
		}
		cout << "time cost for erase:   " << sw() << endl;
		cout << mp.size() << endl;
	}
	cout << "Ajy::avltree_map" << endl;
	{
		Ajy::StopWatch sw;
		Ajy::avltree_map<Ajy::String, int> mp;
		for (int i = 0; i < n; ++i)
		{
			mp.emplace(String::MakeString(i), i);
		}
		cout << "time cost for insert:   " << sw() << endl;
		cout << mp.size() << endl;
		long long c = 0;
		sw.Reset();
		for (auto it = mp.begin(); it != mp.end(); ++it)
		{
			c += it->second;
		}
		cout << "time cost for traverse:   " << sw() << endl;
		cout << c << endl;
		c = 0;
		sw.Reset();
		for (int i = 0; i < n; ++i)
		{
			c += mp.find(String::MakeString(i))->second;
		}
		cout << "time cost for find:   " << sw() << endl;
		cout << c << endl;
		sw.Reset();
		for (auto it = mp.begin(); it != mp.end();)
		{
			it = mp.erase(it);
		}
		cout << "time cost for erase:   " << sw() << endl;
		cout << mp.size() << endl;
	}
	cout << "Ajy::bpptree_map" << endl;
	{
		Ajy::StopWatch sw;
		Ajy::bpptree_map<Ajy::String, int> mp;
		for (int i = 0; i < n; ++i)
		{
			mp.emplace(String::MakeString(i), i);
		}
		cout << "time cost for insert:   " << sw() << endl;
		cout << mp.size() << endl;
		long long c = 0;
		sw.Reset();
		for (auto it = mp.begin(); it != mp.end(); ++it)
		{
			c += it->second;
		}
		cout << "time cost for traverse:   " << sw() << endl;
		cout << c << endl;
		c = 0;
		sw.Reset();
		for (int i = 0; i < n; ++i)
		{
			c += mp.find(String::MakeString(i))->second;
		}
		cout << "time cost for find:   " << sw() << endl;
		cout << c << endl;
		sw.Reset();
		for (auto it = mp.begin(); it != mp.end();)
		{
			it = mp.erase(it);
		}
		cout << "time cost for erase:   " << sw() << endl;
		cout << mp.size() << endl;
	}
	cout << "Ajy::sbtree_map" << endl;
	{
		Ajy::StopWatch sw;
		Ajy::sbtree_multimap<Ajy::String, int> mp;
		for (int i = 0; i < n; ++i)
		{
			mp.emplace(String::MakeString(i), i);
		}
		cout << "time cost for insert:   " << sw() << endl;
		cout << mp.size() << endl;
		long long c = 0;
		sw.Reset();
		for (auto it = mp.begin(); it != mp.end(); ++it)
		{
			c += it->second;
		}
		cout << "time cost for traverse:   " << sw() << endl;
		cout << c << endl;
		c = 0;
		sw.Reset();
		for (int i = 0; i < n; ++i)
		{
			c += mp.find(String::MakeString(i))->second;
		}
		cout << "time cost for find:   " << sw() << endl;
		cout << c << endl;
		sw.Reset();
		for (auto it = mp.begin(); it != mp.end();)
		{
			it = mp.erase(it);
		}
		cout << "time cost for erase:   " << sw() << endl;
		cout << mp.size() << endl;
	}
}

void Map_Compare()
{
	Map_Compare_1();
	//Map_Compare_2();
	//Map_Compare_3();
}
