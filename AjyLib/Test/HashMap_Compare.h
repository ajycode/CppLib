#pragma once

void HashMap_Compare_1()
{
	int n = 15000000;
	cout << "unordered_map<int,int>" << endl;
	std::cout << "{" << std::endl;
	{
		cout << "std::hash" << endl;
		std::cout << "{" << std::endl;
		{
			Ajy::StopWatch sw;
			std::unordered_map<int, int> m;
			for (int i = 0; i < n; ++i)
			{
				m.emplace(i, i);
			}
			cout << "time cost for insert:   " << sw() << endl;
			cout << m.size() << endl;
			long long c = 0;
			sw.Reset();
			for (auto it = m.begin(); it != m.end(); ++it)
			{
				c += it->second;
			}
			cout << "time cost for traverse:   " << sw() << endl;
			cout << c << endl;
			c = 0;
			sw.Reset();
			for (int i = 0; i < n; ++i)
			{
				c += m.find(i)->second;
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
		std::cout << "}" << std::endl;
		cout << "Hasher" << endl;
		std::cout << "{" << std::endl;
		{
			Ajy::StopWatch sw;
			std::unordered_map<int, int, Hasher<int>> m;
			for (int i = 0; i < n; ++i)
			{
				m.emplace(i, i);
			}
			cout << "time cost for insert:   " << sw() << endl;
			cout << m.size() << endl;
			long long c = 0;
			sw.Reset();
			for (auto it = m.begin(); it != m.end(); ++it)
			{
				c += it->second;
			}
			cout << "time cost for traverse:   " << sw() << endl;
			cout << c << endl;
			c = 0;
			sw.Reset();
			for (int i = 0; i < n; ++i)
			{
				c += m.find(i)->second;
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
		std::cout << "}" << std::endl;
	}
	std::cout << "}" << std::endl;
	cout << "cahash_map<int,int>" << endl;
	std::cout << "{" << std::endl;
	{
		cout << "std::hash" << endl;
		std::cout << "{" << std::endl;
		{
			Ajy::StopWatch sw;
			Ajy::cahash_map<int, int, std::hash<int>> m;
			for (int i = 0; i < n; ++i)
			{
				m.emplace(i, i);
			}
			cout << "time cost for insert:   " << sw() << endl;
			cout << m.size() << endl;
			long long c = 0;
			sw.Reset();
			for (auto it = m.begin(); it != m.end(); ++it)
			{
				c += it->second;
			}
			cout << "time cost for traverse:   " << sw() << endl;
			cout << c << endl;
			c = 0;
			sw.Reset();
			for (int i = 0; i < n; ++i)
			{
				c += m.find(i)->second;
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
		std::cout << "}" << std::endl;
		cout << "Hasher" << endl;
		std::cout << "{" << std::endl;
		{
			Ajy::StopWatch sw;
			Ajy::cahash_map<int, int> m;
			for (int i = 0; i < n; ++i)
			{
				m.emplace(i, i);
			}
			cout << "time cost for insert:   " << sw() << endl;
			cout << m.size() << endl;
			long long c = 0;
			sw.Reset();
			for (auto it = m.begin(); it != m.end(); ++it)
			{
				c += it->second;
			}
			cout << "time cost for traverse:   " << sw() << endl;
			cout << c << endl;
			c = 0;
			sw.Reset();
			for (int i = 0; i < n; ++i)
			{
				c += m.find(i)->second;
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
		std::cout << "}" << std::endl;
	}
	std::cout << "}" << std::endl;
	cout << "Dictionary<int,int>" << endl;
	std::cout << "{" << std::endl;
	{
		Ajy::StopWatch sw;
		Ajy::Dictionary<int, int> m;
		for (int i = 0; i < n; ++i)
		{
			m.EmplaceAdd(false, i, i);
		}
		cout << "time cost for insert:   " << sw() << endl;
		cout << m.Size() << endl;
		long long c = 0;
		sw.Reset();
		for (int j = 0; j < m.count; ++j)
		{
			if (m.entries[j].hashCode < 0) continue;
			c += m.IndexAt(j);
		}
		cout << "time cost for traverse:   " << sw() << endl;
		cout << c << endl;
		c = 0;
		sw.Reset();
		for (int i = 0; i < n; ++i)
		{
			c += m.IndexAt(m.Find(i));
		}
		cout << "time cost for find:   " << sw() << endl;
		cout << c << endl;
		sw.Reset();
		for (int j = 0; j < m.count; ++j)
		{
			if (m.entries[j].hashCode < 0) continue;
			m.RemoveIndexAt(j);
		}
		cout << "time cost for erase:   " << sw() << endl;
		cout << m.Size() << endl;
	}
	std::cout << "}" << std::endl;
	cout << "CMHashMap<int,int>" << endl;
	std::cout << "{" << std::endl;
	{
		Ajy::StopWatch sw;
		Ajy::CMHashMap<int, int> m;
		for (int i = 0; i < n; ++i)
		{
			m.EmplaceAdd(false, i, i);
		}
		cout << "time cost for insert:   " << sw() << endl;
		cout << m.Size() << endl;
		long long c = 0;
		sw.Reset();
		for (int j = 0; j < m.count; ++j)
		{
			if (m.items[j].prev == -2) continue;
			c += m.IndexAtValue(j);
		}
		cout << "time cost for traverse:   " << sw() << endl;
		cout << c << endl;
		c = 0;
		sw.Reset();
		for (int i = 0; i < n; ++i)
		{
			c += m.IndexAtValue(m.Find(i));
		}
		cout << "time cost for find:   " << sw() << endl;
		cout << c << endl;
		sw.Reset();
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
	std::cout << "}" << std::endl;
	cout << "HashMap<int,int>" << endl;
	std::cout << "{" << std::endl;
	{
		Ajy::StopWatch sw;
		Ajy::HashMap<int, int> m;
		for (int i = 0; i < n; ++i)
		{
			m.EmplaceAdd(false, i, i);
		}
		cout << "time cost for insert:   " << sw() << endl;
		cout << m.Size() << endl;
		long long c = 0;
		sw.Reset();
		auto data = m.Data();
		for (int j = 0; j < data.DataLen(); ++j)
		{
			c += data[j]->value;
		}
		cout << "time cost for traverse:   " << sw() << endl;
		cout << c << endl;
		c = 0;
		sw.Reset();
		for (int i = 0; i < n; ++i)
		{
			c += m.Find(i)->value;
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
	std::cout << "}" << std::endl;
	cout << "==============================================================" << endl;
	std::vector<int> vec;
	for (int i = 0; i < n; ++i)
	{
		vec.emplace_back(i);
	}
	std::random_shuffle(vec.begin(), vec.end());
	cout << "unordered_map<int,int>" << endl;
	std::cout << "{" << std::endl;
	{
		cout << "std::hash" << endl;
		std::cout << "{" << std::endl;
		{
			Ajy::StopWatch sw;
			std::unordered_map<int, int> m;
			for (int i = 0; i < n; ++i)
			{
				m.emplace(vec[i], vec[i]);
			}
			cout << "time cost for insert:   " << sw() << endl;
			cout << m.size() << endl;
			long long c = 0;
			sw.Reset();
			for (auto it = m.begin(); it != m.end(); ++it)
			{
				c += it->second;
			}
			cout << "time cost for traverse:   " << sw() << endl;
			cout << c << endl;
			c = 0;
			sw.Reset();
			for (int i = 0; i < n; ++i)
			{
				c += m.find(vec[i])->second;
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
		std::cout << "}" << std::endl;
		cout << "Hasher" << endl;
		std::cout << "{" << std::endl;
		{
			Ajy::StopWatch sw;
			std::unordered_map<int, int, Hasher<int>> m;
			for (int i = 0; i < n; ++i)
			{
				m.emplace(vec[i], vec[i]);
			}
			cout << "time cost for insert:   " << sw() << endl;
			cout << m.size() << endl;
			long long c = 0;
			sw.Reset();
			for (auto it = m.begin(); it != m.end(); ++it)
			{
				c += it->second;
			}
			cout << "time cost for traverse:   " << sw() << endl;
			cout << c << endl;
			c = 0;
			sw.Reset();
			for (int i = 0; i < n; ++i)
			{
				c += m.find(vec[i])->second;
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
		std::cout << "}" << std::endl;
	}
	std::cout << "}" << std::endl;
	cout << "cahash_map<int,int>" << endl;
	std::cout << "{" << std::endl;
	{
		cout << "std::hash" << endl;
		std::cout << "{" << std::endl;
		{
			Ajy::StopWatch sw;
			Ajy::cahash_map<int, int, std::hash<int>> m;
			for (int i = 0; i < n; ++i)
			{
				m.emplace(vec[i], vec[i]);
			}
			cout << "time cost for insert:   " << sw() << endl;
			cout << m.size() << endl;
			long long c = 0;
			sw.Reset();
			for (auto it = m.begin(); it != m.end(); ++it)
			{
				c += it->second;
			}
			cout << "time cost for traverse:   " << sw() << endl;
			cout << c << endl;
			c = 0;
			sw.Reset();
			for (int i = 0; i < n; ++i)
			{
				c += m.find(vec[i])->second;
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
		std::cout << "}" << std::endl;
		cout << "Hasher" << endl;
		std::cout << "{" << std::endl;
		{
			Ajy::StopWatch sw;
			Ajy::cahash_map<int, int> m;
			for (int i = 0; i < n; ++i)
			{
				m.emplace(vec[i], vec[i]);
			}
			cout << "time cost for insert:   " << sw() << endl;
			cout << m.size() << endl;
			long long c = 0;
			sw.Reset();
			for (auto it = m.begin(); it != m.end(); ++it)
			{
				c += it->second;
			}
			cout << "time cost for traverse:   " << sw() << endl;
			cout << c << endl;
			c = 0;
			sw.Reset();
			for (int i = 0; i < n; ++i)
			{
				c += m.find(vec[i])->second;
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
		std::cout << "}" << std::endl;
	}
	std::cout << "}" << std::endl;
	cout << "Dictionary<int,int>" << endl;
	std::cout << "{" << std::endl;
	{
		Ajy::StopWatch sw;
		Ajy::Dictionary<int, int> m;
		for (int i = 0; i < n; ++i)
		{
			m.EmplaceAdd(false, vec[i], vec[i]);
		}
		cout << "time cost for insert:   " << sw() << endl;
		cout << m.Size() << endl;
		long long c = 0;
		sw.Reset();
		for (int j = 0; j < m.count; ++j)
		{
			if (m.entries[j].hashCode < 0) continue;
			c += m.IndexAt(j);
		}
		cout << "time cost for traverse:   " << sw() << endl;
		cout << c << endl;
		c = 0;
		sw.Reset();
		for (int i = 0; i < n; ++i)
		{
			c += m.IndexAt(m.Find(vec[i]));
		}
		cout << "time cost for find:   " << sw() << endl;
		cout << c << endl;
		sw.Reset();
		for (int j = 0; j < m.count; ++j)
		{
			if (m.entries[j].hashCode < 0) continue;
			m.RemoveIndexAt(j);
		}
		cout << "time cost for erase:   " << sw() << endl;
		cout << m.Size() << endl;
	}
	std::cout << "}" << std::endl;
	cout << "CMHashMap<int,int>" << endl;
	std::cout << "{" << std::endl;
	{
		Ajy::StopWatch sw;
		Ajy::CMHashMap<int, int> m;
		for (int i = 0; i < n; ++i)
		{
			m.EmplaceAdd(false, vec[i], vec[i]);
		}
		cout << "time cost for insert:   " << sw() << endl;
		cout << m.Size() << endl;
		long long c = 0;
		sw.Reset();
		for (int j = 0; j < m.count; ++j)
		{
			if (m.items[j].prev == -2) continue;
			c += m.IndexAtValue(j);
		}
		cout << "time cost for traverse:   " << sw() << endl;
		cout << c << endl;
		c = 0;
		sw.Reset();
		for (int i = 0; i < n; ++i)
		{
			c += m.IndexAtValue(m.Find(vec[i]));
		}
		cout << "time cost for find:   " << sw() << endl;
		cout << c << endl;
		sw.Reset();
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
	std::cout << "}" << std::endl;
	cout << "HashMap<int,int>" << endl;
	std::cout << "{" << std::endl;
	{
		Ajy::StopWatch sw;
		Ajy::HashMap<int, int> m;
		for (int i = 0; i < n; ++i)
		{
			m.EmplaceAdd(false, vec[i], vec[i]);
		}
		cout << "time cost for insert:   " << sw() << endl;
		cout << m.Size() << endl;
		long long c = 0;
		sw.Reset();
		auto data = m.Data();
		for (int j = 0; j < data.DataLen(); ++j)
		{
			c += data[j]->value;
		}
		cout << "time cost for traverse:   " << sw() << endl;
		cout << c << endl;
		c = 0;
		sw.Reset();
		for (int i = 0; i < n; ++i)
		{
			c += m.Find(vec[i])->value;
		}
		cout << "time cost for find:   " << sw() << endl;
		cout << c << endl;
		sw.Reset();
		for (int i = 0; i < n; ++i)
		{
			m.Remove(vec[i]);
		}
		cout << "time cost for erase:   " << sw() << endl;
		cout << m.Size() << endl;
	}
	std::cout << "}" << std::endl;
}

void HashMap_Compare_2()
{
	int n = 15000000;
	cout << "unordered_map<int,String>" << endl;
	{
		Ajy::StopWatch sw;
		std::unordered_map<int, String> m;
		for (int i = 0; i < n; ++i)
		{
			m.emplace(i, String::MakeString(i));
		}
		cout << "time cost for insert:   " << sw() << endl;
		cout << m.size() << endl;
		long long c = 0;
		sw.Reset();
		for (auto it = m.begin(); it != m.end(); ++it)
		{
			c += it->first;
		}
		cout << "time cost for traverse:   " << sw() << endl;
		cout << c << endl;
		sw.Reset();
		for (int i = 0; i < n; ++i)
		{
			m.erase(i);
		}
		cout << "time cost for erase:   " << sw() << endl;
		cout << m.size() << endl;
	}
	cout << "cahash_map<int,String>" << endl;
	{
		Ajy::StopWatch sw;
		Ajy::cahash_map<int, String> m;
		for (int i = 0; i < n; ++i)
		{
			m.emplace(i, String::MakeString(i));
		}
		cout << "time cost for insert:   " << sw() << endl;
		cout << m.size() << endl;
		long long c = 0;
		sw.Reset();
		for (auto it = m.begin(); it != m.end(); ++it)
		{
			c += it->first;
		}
		cout << "time cost for traverse:   " << sw() << endl;
		cout << c << endl;
		sw.Reset();
		for (int i = 0; i < n; ++i)
		{
			m.erase(i);
		}
		cout << "time cost for erase:   " << sw() << endl;
		cout << m.size() << endl;
	}
	cout << "Dictionary<int,String>" << endl;
	{
		Ajy::StopWatch sw;
		Ajy::Dictionary<int, String> m;
		for (int i = 0; i < n; ++i)
		{
			m.EmplaceAdd(false, i, String::MakeString(i));
		}
		cout << "time cost for insert:   " << sw() << endl;
		cout << m.Size() << endl;
		long long c = 0;
		sw.Reset();
		for (int j = 0; j < m.count; ++j)
		{
			if (m.entries[j].hashCode < 0) continue;
			c += j;
		}
		cout << "time cost for traverse:   " << sw() << endl;
		cout << c << endl;
		sw.Reset();
		for (int i = 0; i < n; ++i)
		{
			m.Remove(i);
		}
		cout << "time cost for erase:   " << sw() << endl;
		cout << m.Size() << endl;
	}
	cout << "CMHashMap<int,String>" << endl;
	{
		Ajy::StopWatch sw;
		Ajy::CMHashMap<int, String> m;
		for (int i = 0; i < n; ++i)
		{
			m.EmplaceAdd(false, i, String::MakeString(i));
		}
		cout << "time cost for insert:   " << sw() << endl;
		cout << m.Size() << endl;
		long long c = 0;
		sw.Reset();
		for (int j = 0; j < m.count; ++j)
		{
			if (m.items[j].prev == -2) continue;
			c += j;
		}
		cout << "time cost for traverse:   " << sw() << endl;
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
	cout << "HashMap<int,String>" << endl;
	{
		Ajy::StopWatch sw;
		Ajy::HashMap<int, String> m;
		for (int i = 0; i < n; ++i)
		{
			m.EmplaceAdd(false, i, String::MakeString(i));
		}
		cout << "time cost for insert:   " << sw() << endl;
		cout << m.Size() << endl;
		long long c = 0;
		sw.Reset();
		auto data = m.Data();
		for (int j = 0; j < data.DataLen(); ++j)
		{
			if (data[j]->hash < 0) continue;
			c += data[j]->key;
		}
		cout << "time cost for traverse:   " << sw() << endl;
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

void HashMap_Compare_3()
{
	int n = 15000000;
	cout << "unordered_map<std::string,Ajy::String>" << endl;
	std::cout << "{" << std::endl;
	{
		cout << "std::hash" << endl;
		std::cout << "{" << std::endl;
		{
			Ajy::StopWatch sw;
			std::unordered_map<std::string, Ajy::String> m;
			for (int i = 0; i < n; ++i)
			{
				char buf[10];
				sprintf(buf, "%d", i);
				m.emplace(std::string(buf), String::MakeString(i));
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
			for (int i = 0; i < n; ++i)
			{
				char buf[10];
				sprintf(buf, "%d", i);
				auto it = m.find(std::string(buf));
				assert(it != m.end());
			}
			cout << "time cost for find:   " << sw() << endl;
			sw.Reset();
			for (int i = 0; i < n; ++i)
			{
				char buf[10];
				sprintf(buf, "%d", i);
				m.erase(std::string(buf));
			}
			cout << "time cost for erase:   " << sw() << endl;
			cout << m.size() << endl;
		}
		std::cout << "}" << std::endl;
		cout << "Hasher" << endl;
		std::cout << "{" << std::endl;
		{
			Ajy::StopWatch sw;
			std::unordered_map<std::string, Ajy::String, Hasher<std::string>> m;
			for (int i = 0; i < n; ++i)
			{
				char buf[10];
				sprintf(buf, "%d", i);
				m.emplace(std::string(buf), String::MakeString(i));
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
			for (int i = 0; i < n; ++i)
			{
				char buf[10];
				sprintf(buf, "%d", i);
				auto it = m.find(std::string(buf));
				assert(it != m.end());
			}
			cout << "time cost for find:   " << sw() << endl;
			sw.Reset();
			for (int i = 0; i < n; ++i)
			{
				char buf[10];
				sprintf(buf, "%d", i);
				m.erase(std::string(buf));
			}
			cout << "time cost for erase:   " << sw() << endl;
			cout << m.size() << endl;
		}
		std::cout << "}" << std::endl;
	}
	std::cout << "}" << std::endl;
	cout << "cahash_map<std::string,Ajy::String>" << endl;
	std::cout << "{" << std::endl;
	{
		cout << "std::hash" << endl;
		std::cout << "{" << std::endl;
		{
			Ajy::StopWatch sw;
			Ajy::cahash_map<std::string, Ajy::String, std::hash<std::string>> m;
			for (int i = 0; i < n; ++i)
			{
				char buf[10];
				sprintf(buf, "%d", i);
				m.emplace(std::string(buf), String::MakeString(i));
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
			for (int i = 0; i < n; ++i)
			{
				char buf[10];
				sprintf(buf, "%d", i);
				auto it = m.find(std::string(buf));
				assert(it != m.end());
			}
			cout << "time cost for find:   " << sw() << endl;
			sw.Reset();
			for (int i = 0; i < n; ++i)
			{
				char buf[10];
				sprintf(buf, "%d", i);
				m.erase(std::string(buf));
			}
			cout << "time cost for erase:   " << sw() << endl;
			cout << m.size() << endl;
		}
		std::cout << "}" << std::endl;
		cout << "Hasher" << endl;
		std::cout << "{" << std::endl;
		{
			Ajy::StopWatch sw;
			Ajy::cahash_map<std::string, Ajy::String> m;
			for (int i = 0; i < n; ++i)
			{
				char buf[10];
				sprintf(buf, "%d", i);
				m.emplace(std::string(buf), String::MakeString(i));
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
			for (int i = 0; i < n; ++i)
			{
				char buf[10];
				sprintf(buf, "%d", i);
				auto it = m.find(std::string(buf));
				assert(it != m.end());
			}
			cout << "time cost for find:   " << sw() << endl;
			sw.Reset();
			for (int i = 0; i < n; ++i)
			{
				char buf[10];
				sprintf(buf, "%d", i);
				m.erase(std::string(buf));
			}
			cout << "time cost for erase:   " << sw() << endl;
			cout << m.size() << endl;
		}
		std::cout << "}" << std::endl;
	}
	std::cout << "}" << std::endl;
	cout << "Dictionary<std::string,Ajy::String>" << endl;
	std::cout << "{" << std::endl;
	{
		Ajy::StopWatch sw;
		Ajy::Dictionary<std::string, String> m;
		for (int i = 0; i < n; ++i)
		{
			char buf[10];
			sprintf(buf, "%d", i);
			m.EmplaceAdd(false, std::string(buf), String::MakeString(i));
		}
		cout << "time cost for insert:   " << sw() << endl;
		cout << m.Size() << endl;
		long long c = 0;
		sw.Reset();
		for (int j = 0; j < m.count; ++j)
		{
			if (m.entries[j].hashCode < 0) continue;
			c += 1;
		}
		cout << "time cost for traverse:   " << sw() << endl;
		cout << c << endl;
		sw.Reset();
		for (int i = 0; i < n; ++i)
		{
			char buf[10];
			sprintf(buf, "%d", i);
			auto it = m.Find(std::string(buf));
			assert(it != -1);
		}
		cout << "time cost for find:   " << sw() << endl;
		sw.Reset();
		for (int i = 0; i < n; ++i)
		{
			char buf[10];
			sprintf(buf, "%d", i);
			m.Remove(std::string(buf));
		}
		cout << "time cost for erase:   " << sw() << endl;
		cout << m.Size() << endl;
	}
	std::cout << "}" << std::endl;
	cout << "CMHashMap<std::string,Ajy::String>" << endl;
	std::cout << "{" << std::endl;
	{
		Ajy::StopWatch sw;
		Ajy::CMHashMap<std::string, String> m;
		for (int i = 0; i < n; ++i)
		{
			char buf[10];
			sprintf(buf, "%d", i);
			m.EmplaceAdd(false, std::string(buf), String::MakeString(i));
		}
		cout << "time cost for insert:   " << sw() << endl;
		cout << m.Size() << endl;
		long long c = 0;
		sw.Reset();
		for (int j = 0; j < m.count; ++j)
		{
			if (m.items[j].prev == -2) continue;
			c += 1;
		}
		cout << "time cost for traverse:   " << sw() << endl;
		cout << c << endl;
		sw.Reset();
		for (int i = 0; i < n; ++i)
		{
			char buf[10];
			sprintf(buf, "%d", i);
			auto it = m.Find(std::string(buf));
			assert(it != -1);
		}
		cout << "time cost for find:   " << sw() << endl;
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
	std::cout << "}" << std::endl;
	cout << "HashMap<std::string,Ajy::String>" << endl;
	std::cout << "{" << std::endl;
	{
		Ajy::StopWatch sw;
		Ajy::HashMap<std::string, String> m;
		for (int i = 0; i < n; ++i)
		{
			char buf[10];
			sprintf(buf, "%d", i);
			m.EmplaceAdd(false, std::string(buf), String::MakeString(i));
		}
		cout << "time cost for insert:   " << sw() << endl;
		cout << m.Size() << endl;
		long long c = 0;
		sw.Reset();
		auto data = m.Data();
		for (int j = 0; j < data.DataLen(); ++j)
		{
			if (data[j]->hash < 0) continue;
			c += 1;
		}
		cout << "time cost for traverse:   " << sw() << endl;
		cout << c << endl;
		sw.Reset();
		for (int i = 0; i < n; ++i)
		{
			char buf[10];
			sprintf(buf, "%d", i);
			auto it = m.Find(std::string(buf));
			assert(it != nullptr);
		}
		cout << "time cost for find:   " << sw() << endl;
		sw.Reset();
		for (int i = 0; i < n; ++i)
		{
			char buf[10];
			sprintf(buf, "%d", i);
			m.Remove(std::string(buf));
		}
		cout << "time cost for erase:   " << sw() << endl;
		cout << m.Size() << endl;
	}
	std::cout << "}" << std::endl;
}

void HashMap_Compare()
{
	HashMap_Compare_1();
	//HashMap_Compare_2();
	//HashMap_Compare_3();
}
