#pragma once

void HashMultiMap_Compare_1()
{
	std::mt19937 mt(0);
	auto mtr = std::uniform_int_distribution<int>(-1000000, 1000000);

	std::unordered_multimap<int, int> stdhashmap_stdhash;
	cahash_multimap<int, int, std::hash<int>> cmhashmap_stdhash;
	std::unordered_multimap<int, int, Hasher<int>> stdhashmap_Hasher;
	cahash_multimap<int, int> cmhashmap_Hasher;


	std::vector<int> v;
	v.resize(20000000);
	auto reset = [&mtr, &mt, &v]()
	{
		for (auto& value : v)
		{
			value = mtr(mt);
		}
	};

	auto testcm_stdhash = [&mtr, &mt, &c = cmhashmap_stdhash, &v]()
	{
		cout << "Ajy::cahash_multimap<int,int>,hash_function->std::hash" << endl;
		cout << "{" << endl;
		StopWatch sw;
		for (int i = 0; i < int(v.size()); ++i)
		{
			c.emplace(v[i], v[i]);
		}
		for (int i = 0; i < int(v.size()); ++i)
		{
			c.emplace(i, v[i]);
		}
		cout << "time cost(ms) for insert:   " << sw() << endl;
		cout << "size:   " << c.size() << endl;
		sw.Reset();
		long long val = 0;
		for (auto it = c.begin(); it != c.end(); ++it)
		{
			val += it->second;
		}
		cout << "time cost(ms) for traverse:   " << sw() << endl;
		cout << "val:   " << val << endl;
		sw.Reset();
		for (auto it = c.begin(); it != c.end();)
		{
			it = c.erase(it);
		}
		/*for (int i = 0; i < int(v.size()); ++i)
		{
		c.erase(v[i]);
		}
		for (int i = 0; i < int(v.size()); ++i)
		{
		c.erase(i);
		}*/
		cout << "time cost(ms) for erase:   " << sw() << endl;
		cout << "size:   " << c.size() << endl;
		cout << "}" << endl;
		c.clear();
	};
	auto teststd_stdhash = [&mtr, &mt, &c = stdhashmap_stdhash, &v]()
	{
		cout << "std::unordered_multimap<int,int>,hash_function->std::hash" << endl;
		cout << "{" << endl;
		StopWatch sw;
		for (int i = 0; i < int(v.size()); ++i)
		{
			c.emplace(v[i], v[i]);
		}
		for (int i = 0; i < int(v.size()); ++i)
		{
			c.emplace(i, v[i]);
		}
		cout << "time cost(ms) for insert:   " << sw() << endl;
		cout << "size:   " << c.size() << endl;
		sw.Reset();
		long long val = 0;
		for (auto it = c.begin(); it != c.end(); ++it)
		{
			val += it->second;
		}
		cout << "time cost(ms) for traverse:   " << sw() << endl;
		cout << "val:   " << val << endl;
		sw.Reset();
		for (auto it = c.begin(); it != c.end();)
		{
			it = c.erase(it);
		}
		/*for (int i = 0; i < int(v.size()); ++i)
		{
		c.erase(v[i]);
		}
		for (int i = 0; i < int(v.size()); ++i)
		{
		c.erase(i);
		}*/
		cout << "time cost(ms) for erase:   " << sw() << endl;
		cout << "size:   " << c.size() << endl;
		cout << "}" << endl;
		c.clear();
	};

	auto testcm_Hasher = [&mtr, &mt, &c = cmhashmap_Hasher, &v]()
	{
		cout << "Ajy::cahash_multimap<int,int>,hash_function->Ajy::Hasher" << endl;
		cout << "{" << endl;
		StopWatch sw;
		for (int i = 0; i < int(v.size()); ++i)
		{
			c.emplace(v[i], v[i]);
		}
		for (int i = 0; i < int(v.size()); ++i)
		{
			c.emplace(i, v[i]);
		}
		cout << "time cost(ms) for insert:   " << sw() << endl;
		cout << "size:   " << c.size() << endl;
		sw.Reset();
		long long val = 0;
		for (auto it = c.begin(); it != c.end(); ++it)
		{
			val += it->second;
		}
		cout << "time cost(ms) for traverse:   " << sw() << endl;
		cout << "val:   " << val << endl;
		sw.Reset();
		for (auto it = c.begin(); it != c.end();)
		{
			it = c.erase(it);
		}
		/*for (int i = 0; i < int(v.size()); ++i)
		{
		c.erase(v[i]);
		}
		for (int i = 0; i < int(v.size()); ++i)
		{
		c.erase(i);
		}*/
		cout << "time cost(ms) for erase:   " << sw() << endl;
		cout << "size:   " << c.size() << endl;
		cout << "}" << endl;
		c.clear();
	};
	auto teststd_Hasher = [&mtr, &mt, &c = stdhashmap_Hasher, &v]()
	{
		cout << "std::unordered_multimap<int,int>,hash_function->Ajy::Hasher" << endl;
		cout << "{" << endl;
		StopWatch sw;
		for (int i = 0; i < int(v.size()); ++i)
		{
			c.emplace(v[i], v[i]);
		}
		for (int i = 0; i < int(v.size()); ++i)
		{
			c.emplace(i, v[i]);
		}
		cout << "time cost(ms) for insert:   " << sw() << endl;
		cout << "size:   " << c.size() << endl;
		sw.Reset();
		long long val = 0;
		for (auto it = c.begin(); it != c.end(); ++it)
		{
			val += it->second;
		}
		cout << "time cost(ms) for traverse:   " << sw() << endl;
		cout << "val:   " << val << endl;
		sw.Reset();
		for (auto it = c.begin(); it != c.end();)
		{
			it = c.erase(it);
		}
		/*for (int i = 0; i < int(v.size()); ++i)
		{
		c.erase(v[i]);
		}
		for (int i = 0; i < int(v.size()); ++i)
		{
		c.erase(i);
		}*/
		cout << "time cost(ms) for erase:   " << sw() << endl;
		cout << "size:   " << c.size() << endl;
		cout << "}" << endl;
		c.clear();
	};



	reset();
	testcm_stdhash();
	teststd_stdhash();
	reset();
	testcm_stdhash();
	teststd_stdhash();
	reset();
	testcm_stdhash();
	teststd_stdhash();

	cout << "=====================================" << endl;

	reset();
	testcm_Hasher();
	teststd_Hasher();
	reset();
	testcm_Hasher();
	teststd_Hasher();
	reset();
	testcm_Hasher();
	teststd_Hasher();

	v.clear();
}

void HashMultiMap_Compare_2()
{
	std::mt19937 mt(0);
	auto mtr = std::uniform_int_distribution<int>(-1000000, 1000000);

	std::unordered_multimap<std::string, Ajy::String> stdhashmap_stdhash;
	cahash_multimap<std::string, Ajy::String, std::hash<std::string>> cmhashmap_stdhash;
	std::unordered_multimap<std::string, Ajy::String, Hasher<std::string>> stdhashmap_Hasher;
	cahash_multimap<std::string, Ajy::String> cmhashmap_Hasher;


	std::vector<int> v;
	v.resize(20000000);
	auto reset = [&mtr, &mt, &v]()
	{
		for (auto &value : v)
		{
			value = mtr(mt);
		}
	};

	auto testcm_stdhash = [&mtr, &mt, &c = cmhashmap_stdhash, &v]()
	{
		cout << "Ajy::cahash_multimap<std::string,Ajy::String>,hash_function->std::hash" << endl;
		cout << "{" << endl;
		StopWatch sw;
		for (int i = 0; i < int(v.size()); ++i)
		{
			char str[10];
			sprintf(str, "%d", v[i]);
			c.emplace(std::string(str), Ajy::String::MakeString(v[i]));
		}
		for (int i = 0; i < int(v.size()); ++i)
		{
			char str[10];
			sprintf(str, "%d", i);
			c.emplace(std::string(str), Ajy::String::MakeString(v[i]));
		}
		cout << "time cost(ms) for insert:   " << sw() << endl;
		cout << "size:   " << c.size() << endl;
		sw.Reset();
		long long val = 0;
		for (auto it = c.begin(); it != c.end(); ++it)
		{
			int64 v = 0;
			Ajy::FromString(v, it->second.C_str());
			val += v;
		}
		cout << "time cost(ms) for traverse:   " << sw() << endl;
		cout << "val:   " << val << endl;
		sw.Reset();
		for (auto it = c.begin(); it != c.end();)
		{
			it = c.erase(it);
		}
		cout << "time cost(ms) for erase:   " << sw() << endl;
		cout << "size:   " << c.size() << endl;
		cout << "}" << endl;
		c.clear();
	};
	auto teststd_stdhash = [&mtr, &mt, &c = stdhashmap_stdhash, &v]()
	{
		cout << "std::unordered_multimap<std::string,Ajy::String>,hash_function->std::hash" << endl;
		cout << "{" << endl;
		StopWatch sw;
		for (int i = 0; i < int(v.size()); ++i)
		{
			char str[10];
			sprintf(str, "%d", v[i]);
			c.emplace(std::string(str), Ajy::String::MakeString(v[i]));
		}
		for (int i = 0; i < int(v.size()); ++i)
		{
			char str[10];
			sprintf(str, "%d", i);
			c.emplace(std::string(str), Ajy::String::MakeString(v[i]));
		}
		cout << "time cost(ms) for insert:   " << sw() << endl;
		cout << "size:   " << c.size() << endl;
		sw.Reset();
		long long val = 0;
		for (auto it = c.begin(); it != c.end(); ++it)
		{
			int64 v = 0;
			Ajy::FromString(v, it->second.C_str());
			val += v;
		}
		cout << "time cost(ms) for traverse:   " << sw() << endl;
		cout << "val:   " << val << endl;
		sw.Reset();
		for (auto it = c.begin(); it != c.end();)
		{
			it = c.erase(it);
		}
		cout << "time cost(ms) for erase:   " << sw() << endl;
		cout << "size:   " << c.size() << endl;
		cout << "}" << endl;
		c.clear();
	};

	auto testcm_Hasher = [&mtr, &mt, &c = cmhashmap_Hasher, &v]()
	{
		cout << "Ajy::cahash_multimap<std::string,Ajy::String>,hash_function->Ajy::Hasher" << endl;
		cout << "{" << endl;
		StopWatch sw;
		for (int i = 0; i < int(v.size()); ++i)
		{
			char str[10];
			sprintf(str, "%d", v[i]);
			c.emplace(std::string(str), Ajy::String::MakeString(v[i]));
		}
		for (int i = 0; i < int(v.size()); ++i)
		{
			char str[10];
			sprintf(str, "%d", i);
			c.emplace(std::string(str), Ajy::String::MakeString(v[i]));
		}
		cout << "time cost(ms) for insert:   " << sw() << endl;
		cout << "size:   " << c.size() << endl;
		sw.Reset();
		long long val = 0;
		for (auto it = c.begin(); it != c.end(); ++it)
		{
			int64 v = 0;
			Ajy::FromString(v, it->second.C_str());
			val += v;
		}
		cout << "time cost(ms) for traverse:   " << sw() << endl;
		cout << "val:   " << val << endl;
		sw.Reset();
		for (auto it = c.begin(); it != c.end();)
		{
			it = c.erase(it);
		}
		cout << "time cost(ms) for erase:   " << sw() << endl;
		cout << "size:   " << c.size() << endl;
		cout << "}" << endl;
		c.clear();
	};
	auto teststd_Hasher = [&mtr, &mt, &c = stdhashmap_Hasher, &v]()
	{
		cout << "std::unordered_multimap<std::string,Ajy::String>,hash_function->Ajy::Hasher" << endl;
		cout << "{" << endl;
		StopWatch sw;
		for (int i = 0; i < int(v.size()); ++i)
		{
			char str[10];
			sprintf(str, "%d", v[i]);
			c.emplace(std::string(str), Ajy::String::MakeString(v[i]));
		}
		for (int i = 0; i < int(v.size()); ++i)
		{
			char str[10];
			sprintf(str, "%d", i);
			c.emplace(std::string(str), Ajy::String::MakeString(v[i]));
		}
		cout << "time cost(ms) for insert:   " << sw() << endl;
		cout << "size:   " << c.size() << endl;
		sw.Reset();
		long long val = 0;
		for (auto it = c.begin(); it != c.end(); ++it)
		{
			int64 v = 0;
			Ajy::FromString(v, it->second.C_str());
			val += v;
		}
		cout << "time cost(ms) for traverse:   " << sw() << endl;
		cout << "val:   " << val << endl;
		sw.Reset();
		for (auto it = c.begin(); it != c.end();)
		{
			it = c.erase(it);
		}
		cout << "time cost(ms) for erase:   " << sw() << endl;
		cout << "size:   " << c.size() << endl;
		cout << "}" << endl;
		c.clear();
	};



	reset();
	testcm_stdhash();
	teststd_stdhash();
	reset();
	testcm_stdhash();
	teststd_stdhash();
	reset();
	testcm_stdhash();
	teststd_stdhash();

	cout << "=====================================" << endl;

	reset();
	testcm_Hasher();
	teststd_Hasher();
	reset();
	testcm_Hasher();
	teststd_Hasher();
	reset();
	testcm_Hasher();
	teststd_Hasher();

	v.clear();
}

void HashMultiMap_Compare()
{
	HashMultiMap_Compare_1();
	//HashMultiMap_Compare_2();
}
