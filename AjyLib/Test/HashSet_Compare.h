#pragma once

void HashSet_Compare_1()
{
	int n = 15000000;
	cout << "unordered_set<int>,std::hash" << endl;
	{
		Ajy::StopWatch sw;
		std::unordered_set<int> m;
		for (int i = 0; i < n; ++i)
		{
			m.emplace(i);
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
		c = 0;
		sw.Reset();
		for (int i = 0; i < n; ++i)
		{
			c += *m.find(i);
		}
		cout << "time cost for find:   " << sw() << endl;
		cout << c << endl;
		sw.Reset();
		for (auto it = m.begin(); it != m.end();)
		{
			it = m.erase(it);
		}
		cout << "time cost for erase:   " << sw() << endl;
		cout << m.size() << endl;
	}
	cout << "unordered_set<int>,Ajy::Hasher" << endl;
	{
		Ajy::StopWatch sw;
		std::unordered_set<int, Hasher<int>> m;
		for (int i = 0; i < n; ++i)
		{
			m.emplace(i);
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
		c = 0;
		sw.Reset();
		for (int i = 0; i < n; ++i)
		{
			c += *m.find(i);
		}
		cout << "time cost for find:   " << sw() << endl;
		cout << c << endl;
		sw.Reset();
		for (auto it = m.begin(); it != m.end();)
		{
			it = m.erase(it);
		}
		cout << "time cost for erase:   " << sw() << endl;
		cout << m.size() << endl;
	}
	cout << "cahash_set<int>,std::hash" << endl;
	{
		Ajy::StopWatch sw;
		Ajy::cahash_set<int, std::hash<int>> m;
		for (int i = 0; i < n; ++i)
		{
			m.emplace(i);
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
		c = 0;
		sw.Reset();
		for (int i = 0; i < n; ++i)
		{
			c += *m.find(i);
		}
		cout << "time cost for find:   " << sw() << endl;
		cout << c << endl;
		sw.Reset();
		for (auto it = m.begin(); it != m.end();)
		{
			it = m.erase(it);
		}
		cout << "time cost for erase:   " << sw() << endl;
		cout << m.size() << endl;
	}
	cout << "cahash_set<int>,Ajy::Hasher" << endl;
	{
		Ajy::StopWatch sw;
		Ajy::cahash_set<int> m;
		for (int i = 0; i < n; ++i)
		{
			m.emplace(i);
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
		c = 0;
		sw.Reset();
		for (int i = 0; i < n; ++i)
		{
			c += *m.find(i);
		}
		cout << "time cost for find:   " << sw() << endl;
		cout << c << endl;
		sw.Reset();
		for (auto it = m.begin(); it != m.end();)
		{
			it = m.erase(it);
		}
		cout << "time cost for erase:   " << sw() << endl;
		cout << m.size() << endl;
	}
	cout << "CMHashSet<int>" << endl;
	{
		Ajy::StopWatch sw;
		Ajy::CMHashSet<int> m;
		for (int i = 0; i < n; ++i)
		{
			m.EmplaceAdd(i);
		}
		cout << "time cost for insert:   " << sw() << endl;
		cout << m.Size() << endl;
		long long c = 0;
		sw.Reset();
		for (int j = 0; j < m.count; ++j)
		{
			if (m.items[j].prev == -2) continue;
			c += m.IndexAtKey(j);
		}
		cout << "time cost for traverse:   " << sw() << endl;
		cout << c << endl;
		c = 0;
		sw.Reset();
		for (int i = 0; i < n; ++i)
		{
			c += m.IndexAtKey(m.Find(i));
		}
		cout << "time cost for find:   " << sw() << endl;
		cout << c << endl;
		sw.Reset();
		/*for (int i = 0; i < n; ++i)
		{
		m.Remove(i);
		}*/
		Vector<int> iters;
		for (int j = 0; j < m.count; ++j)
		{
			if (m.items[j].prev == -2) continue;
			iters.Push(j);
		}
		for (int i = 0; i < n; ++i) m.RemoveIndexAt(iters[i]);
		cout << "time cost for erase:   " << sw() << endl;
		cout << m.Size() << endl;
	}
	cout << "HashSet<int>" << endl;
	{
		Ajy::StopWatch sw;
		Ajy::HashSet<int> m;
		for (int i = 0; i < n; ++i)
		{
			m.EmplaceAdd(i);
		}
		cout << "time cost for insert:   " << sw() << endl;
		cout << m.Size() << endl;
		long long c = 0;
		sw.Reset();
		auto data = m.Data();
		for (int j = 0; j < data.DataLen(); ++j)
		{
			c += data[j]->key;
		}
		cout << "time cost for traverse:   " << sw() << endl;
		cout << c << endl;
		c = 0;
		sw.Reset();
		for (int i = 0; i < n; ++i)
		{
			c += m.Find(i)->key;
		}
		cout << "time cost for find:   " << sw() << endl;
		cout << c << endl;
		sw.Reset();
		for (int i = 0; i < n; ++i)
		{
			m.Remove(i);
		}
		cout << "time cost for erase:   " << sw() << endl;
		cout << m.Size() << endl;
	}
}

void HashSet_Compare_2()
{
	int n = 15000000;
	cout << "unordered_set<std::string>,std::hash" << endl;
	{
		Ajy::StopWatch sw;
		std::unordered_set<std::string> m;
		for (int i = 0; i < n; ++i)
		{
			char str[10];
			sprintf(str, "%d", i);
			m.emplace(std::string(str));
		}
		cout << "time cost for insert:   " << sw() << endl;
		cout << m.size() << endl;
		long long c = 0;
		sw.Reset();
		for (auto it = m.begin(); it != m.end(); ++it)
		{
			uint64 v = 0;
			Ajy::FromString(v, (*it).c_str());
			c += v;
		}
		cout << "time cost for traverse:   " << sw() << endl;
		cout << c << endl;
		c = 0;
		sw.Reset();
		for (int i = 0; i < n; ++i)
		{
			char str[10];
			sprintf(str, "%d", i);
			auto it = m.find(std::string(str));
			if (it!=m.end())
			{
				uint64 v = 0;
				Ajy::FromString(v, (*it).c_str());
				c += v;
			}
		}
		cout << "time cost for find:   " << sw() << endl;
		cout << c << endl;
		sw.Reset();
		for (auto it = m.begin(); it != m.end();)
		{
			it = m.erase(it);
		}
		cout << "time cost for erase:   " << sw() << endl;
		cout << m.size() << endl;
	}
	cout << "unordered_set<std::string>,Ajy::Hasher" << endl;
	{
		Ajy::StopWatch sw;
		std::unordered_set<std::string, Hasher<std::string>> m;
		for (int i = 0; i < n; ++i)
		{
			char str[10];
			sprintf(str, "%d", i);
			m.emplace(std::string(str));
		}
		cout << "time cost for insert:   " << sw() << endl;
		cout << m.size() << endl;
		long long c = 0;
		sw.Reset();
		for (auto it = m.begin(); it != m.end(); ++it)
		{
			uint64 v = 0;
			Ajy::FromString(v, (*it).c_str());
			c += v;
		}
		cout << "time cost for traverse:   " << sw() << endl;
		cout << c << endl;
		c = 0;
		sw.Reset();
		for (int i = 0; i < n; ++i)
		{
			char str[10];
			sprintf(str, "%d", i);
			auto it = m.find(std::string(str));
			if (it != m.end())
			{
				uint64 v = 0;
				Ajy::FromString(v, (*it).c_str());
				c += v;
			}
		}
		cout << "time cost for find:   " << sw() << endl;
		cout << c << endl;
		sw.Reset();
		for (auto it = m.begin(); it != m.end();)
		{
			it = m.erase(it);
		}
		cout << "time cost for erase:   " << sw() << endl;
		cout << m.size() << endl;
	}
	cout << "cahash_set<std::string>,std::hash" << endl;
	{
		Ajy::StopWatch sw;
		Ajy::cahash_set<std::string, std::hash<std::string>> m;
		for (int i = 0; i < n; ++i)
		{
			char str[10];
			sprintf(str, "%d", i);
			m.emplace(std::string(str));
		}
		cout << "time cost for insert:   " << sw() << endl;
		cout << m.size() << endl;
		long long c = 0;
		sw.Reset();
		for (auto it = m.begin(); it != m.end(); ++it)
		{
			uint64 v = 0;
			Ajy::FromString(v, (*it).c_str());
			c += v;
		}
		cout << "time cost for traverse:   " << sw() << endl;
		cout << c << endl;
		c = 0;
		sw.Reset();
		for (int i = 0; i < n; ++i)
		{
			char str[10];
			sprintf(str, "%d", i);
			auto it = m.find(std::string(str));
			if (it != m.end())
			{
				uint64 v = 0;
				Ajy::FromString(v, (*it).c_str());
				c += v;
			}
		}
		cout << "time cost for find:   " << sw() << endl;
		cout << c << endl;
		sw.Reset();
		for (auto it = m.begin(); it != m.end();)
		{
			it = m.erase(it);
		}
		cout << "time cost for erase:   " << sw() << endl;
		cout << m.size() << endl;
	}
	cout << "cahash_set<std::string>,Ajy::Hasher" << endl;
	{
		Ajy::StopWatch sw;
		Ajy::cahash_set<std::string> m;
		for (int i = 0; i < n; ++i)
		{
			char str[10];
			sprintf(str, "%d", i);
			m.emplace(std::string(str));
		}
		cout << "time cost for insert:   " << sw() << endl;
		cout << m.size() << endl;
		long long c = 0;
		sw.Reset();
		for (auto it = m.begin(); it != m.end(); ++it)
		{
			uint64 v = 0;
			Ajy::FromString(v, (*it).c_str());
			c += v;
		}
		cout << "time cost for traverse:   " << sw() << endl;
		cout << c << endl;
		c = 0;
		sw.Reset();
		for (int i = 0; i < n; ++i)
		{
			char str[10];
			sprintf(str, "%d", i);
			auto it = m.find(std::string(str));
			if (it != m.end())
			{
				uint64 v = 0;
				Ajy::FromString(v, (*it).c_str());
				c += v;
			}
		}
		cout << "time cost for find:   " << sw() << endl;
		cout << c << endl;
		sw.Reset();
		for (auto it = m.begin(); it != m.end();)
		{
			it = m.erase(it);
		}
		cout << "time cost for erase:   " << sw() << endl;
		cout << m.size() << endl;
	}
	cout << "CMHashSet<std::string>" << endl;
	{
		Ajy::StopWatch sw;
		Ajy::CMHashSet<std::string> m;
		for (int i = 0; i < n; ++i)
		{
			char str[10];
			sprintf(str, "%d", i);
			m.EmplaceAdd(std::string(str));
		}
		cout << "time cost for insert:   " << sw() << endl;
		cout << m.Size() << endl;
		long long c = 0;
		sw.Reset();
		for (int j = 0; j < m.count; ++j)
		{
			if (m.items[j].prev == -2) continue;
			uint64 v = 0;
			Ajy::FromString(v, m.IndexAtKey(j).c_str());
			c += v;
		}
		cout << "time cost for traverse:   " << sw() << endl;
		cout << c << endl;
		c = 0;
		sw.Reset();
		for (int i = 0; i < n; ++i)
		{
			char str[10];
			sprintf(str, "%d", i);
			auto it = m.Find(std::string(str));
			if (it != -1)
			{
				uint64 v = 0;
				Ajy::FromString(v, m.IndexAtKey(it).c_str());
				c += v;
			}
		}
		cout << "time cost for find:   " << sw() << endl;
		cout << c << endl;
		sw.Reset();
		/*for (int i = 0; i < n; ++i)
		{
		m.Remove(i);
		}*/
		Vector<int> iters;
		for (int j = 0; j < m.count; ++j)
		{
			if (m.items[j].prev == -2) continue;
			iters.Push(j);
		}
		for (int i = 0; i < n; ++i) m.RemoveIndexAt(iters[i]);
		cout << "time cost for erase:   " << sw() << endl;
		cout << m.Size() << endl;
	}
	cout << "HashSet<std::string>" << endl;
	{
		Ajy::StopWatch sw;
		Ajy::HashSet<std::string> m;
		for (int i = 0; i < n; ++i)
		{
			char str[10];
			sprintf(str, "%d", i);
			m.EmplaceAdd(std::string(str));
		}
		cout << "time cost for insert:   " << sw() << endl;
		cout << m.Size() << endl;
		long long c = 0;
		sw.Reset();
		auto data = m.Data();
		for (int j = 0; j < data.DataLen(); ++j)
		{
			uint64 v = 0;
			Ajy::FromString(v, data[j]->key.c_str());
			c += v;
		}
		cout << "time cost for traverse:   " << sw() << endl;
		cout << c << endl;
		c = 0;
		sw.Reset();
		for (int i = 0; i < n; ++i)
		{
			char str[10];
			sprintf(str, "%d", i);
			auto it = m.Find(std::string(str));
			if (it != nullptr)
			{
				uint64 v = 0;
				Ajy::FromString(v, it->key.c_str());
				c += v;
			}
		}
		cout << "time cost for find:   " << sw() << endl;
		cout << c << endl;
		sw.Reset();
		for (int j = 0; j < data.DataLen(); ++j)
		{
			m.Remove(data[j]);
		}
		cout << "time cost for erase:   " << sw() << endl;
		cout << m.Size() << endl;
	}
}

void HashSet_Compare_3()
{
	int n = 15000000;
	cout << "unordered_set<Ajy::String>,Ajy::Hasher" << endl;
	{
		Ajy::StopWatch sw;
		std::unordered_set<Ajy::String, Hasher<Ajy::String>> m;
		for (int i = 0; i < n; ++i)
		{
			m.emplace(Ajy::String::MakeString(i));
		}
		cout << "time cost for insert:   " << sw() << endl;
		cout << m.size() << endl;
		long long c = 0;
		sw.Reset();
		for (auto it = m.begin(); it != m.end(); ++it)
		{
			uint64 v = 0;
			Ajy::FromString(v, (*it).C_str());
			c += v;
		}
		cout << "time cost for traverse:   " << sw() << endl;
		cout << c << endl;
		c = 0;
		sw.Reset();
		for (int i = 0; i < n; ++i)
		{
			auto it = m.find(Ajy::String::MakeString(i));
			if (it != m.end())
			{
				c += i;
			}
		}
		cout << "time cost for find:   " << sw() << endl;
		cout << c << endl;
		sw.Reset();
		for (auto it = m.begin(); it != m.end();)
		{
			it = m.erase(it);
		}
		cout << "time cost for erase:   " << sw() << endl;
		cout << m.size() << endl;
	}
	cout << "cahash_set<Ajy::String>,Ajy::Hasher" << endl;
	{
		Ajy::StopWatch sw;
		Ajy::cahash_set<Ajy::String> m;
		for (int i = 0; i < n; ++i)
		{
			m.emplace(Ajy::String::MakeString(i));
		}
		cout << "time cost for insert:   " << sw() << endl;
		cout << m.size() << endl;
		long long c = 0;
		sw.Reset();
		for (auto it = m.begin(); it != m.end(); ++it)
		{
			uint64 v = 0;
			Ajy::FromString(v, (*it).C_str());
			c += v;
		}
		cout << "time cost for traverse:   " << sw() << endl;
		cout << c << endl;
		c = 0;
		sw.Reset();
		for (int i = 0; i < n; ++i)
		{
			auto it = m.find(Ajy::String::MakeString(i));
			if (it != m.end())
			{
				c += i;
			}
		}
		cout << "time cost for find:   " << sw() << endl;
		cout << c << endl;
		sw.Reset();
		for (auto it = m.begin(); it != m.end();)
		{
			it = m.erase(it);
		}
		cout << "time cost for erase:   " << sw() << endl;
		cout << m.size() << endl;
	}
	cout << "CMHashSet<Ajy::String>" << endl;
	{
		Ajy::StopWatch sw;
		Ajy::CMHashSet<Ajy::String> m;
		for (int i = 0; i < n; ++i)
		{
			m.EmplaceAdd(Ajy::String::MakeString(i));
		}
		cout << "time cost for insert:   " << sw() << endl;
		cout << m.Size() << endl;
		long long c = 0;
		sw.Reset();
		for (int j = 0; j < m.count; ++j)
		{
			if (m.items[j].prev == -2) continue;
			uint64 v = 0;
			Ajy::FromString(v, m.IndexAtKey(j).C_str());
			c += v;
		}
		cout << "time cost for traverse:   " << sw() << endl;
		cout << c << endl;
		c = 0;
		sw.Reset();
		for (int i = 0; i < n; ++i)
		{
			auto it = m.Find(Ajy::String::MakeString(i));
			if (it != -1)
			{
				c += i;
			}
		}
		cout << "time cost for find:   " << sw() << endl;
		cout << c << endl;
		sw.Reset();
		/*for (int i = 0; i < n; ++i)
		{
		m.Remove(i);
		}*/
		Vector<int> iters;
		for (int j = 0; j < m.count; ++j)
		{
			if (m.items[j].prev == -2) continue;
			iters.Push(j);
		}
		for (int i = 0; i < n; ++i) m.RemoveIndexAt(iters[i]);
		cout << "time cost for erase:   " << sw() << endl;
		cout << m.Size() << endl;
	}
	cout << "HashSet<Ajy::String>" << endl;
	{
		Ajy::StopWatch sw;
		Ajy::HashSet<Ajy::String> m;
		for (int i = 0; i < n; ++i)
		{
			m.EmplaceAdd(Ajy::String::MakeString(i));
		}
		cout << "time cost for insert:   " << sw() << endl;
		cout << m.Size() << endl;
		long long c = 0;
		sw.Reset();
		auto data = m.Data();
		for (int j = 0; j < data.DataLen(); ++j)
		{
			uint64 v = 0;
			Ajy::FromString(v, data[j]->key.C_str());
			c += v;
		}
		cout << "time cost for traverse:   " << sw() << endl;
		cout << c << endl;
		c = 0;
		sw.Reset();
		for (int i = 0; i < n; ++i)
		{
			auto it = m.Find(Ajy::String::MakeString(i));
			if (it != nullptr)
			{
				c += i;
			}
		}
		cout << "time cost for find:   " << sw() << endl;
		cout << c << endl;
		sw.Reset();
		for (int j = 0; j < data.DataLen(); ++j)
		{
			m.Remove(data[j]);
		}
		cout << "time cost for erase:   " << sw() << endl;
		cout << m.Size() << endl;
	}
}

void HashSet_Compare()
{
	//HashSet_Compare_1();
	HashSet_Compare_2();
	//HashSet_Compare_3();
}
