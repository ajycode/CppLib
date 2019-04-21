#pragma once

void HashMultiSet_Compare_1()
{
	std::mt19937 mt(0);
	auto mtr = std::uniform_int_distribution<int>(-1000000, 1000000);

	std::unordered_multiset<int> stdhashset_stdhash;
	cahash_multiset<int, std::hash<int>> cmhashset_stdhash;
	std::unordered_multiset<int, Hasher<int>> stdhashset_Hasher;
	cahash_multiset<int> cmhashset_Hasher;


	std::vector<int> v;
	v.resize(20000000);
	auto reset = [&mtr, &mt, &v]()
	{
		for (auto &value : v)
		{
			value = mtr(mt);
		}
	};

	auto testcm_stdhash = [&mtr, &mt, &c = cmhashset_stdhash, &v]()
	{
		cout << "Ajy::cahash_multiset<int>,hash_function->std::hash" << endl;
		cout << "{" << endl;
		StopWatch sw;
		for (int i = 0; i < int(v.size()); ++i)
		{
			c.emplace(v[i]);
		}
		for (int i = 0; i < int(v.size()); ++i)
		{
			c.emplace(i);
		}
		cout << "time cost(ms) for insert:   " << sw() << endl;
		cout << "size:   " << c.size() << endl;
		sw.Reset();
		long long val = 0;
		for (auto it = c.begin(); it != c.end(); ++it)
		{
			val += *it;
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
	auto teststd_stdhash = [&mtr, &mt, &c = stdhashset_stdhash, &v]()
	{
		cout << "std::unordered_multiset<int>,hash_function->std::hash" << endl;
		cout << "{" << endl;
		StopWatch sw;
		for (int i = 0; i < int(v.size()); ++i)
		{
			c.emplace(v[i]);
		}
		for (int i = 0; i < int(v.size()); ++i)
		{
			c.emplace(i);
		}
		cout << "time cost(ms) for insert:   " << sw() << endl;
		cout << "size:   " << c.size() << endl;
		sw.Reset();
		long long val = 0;
		for (auto it = c.begin(); it != c.end(); ++it)
		{
			val += *it;
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

	auto testcm_Hasher = [&mtr, &mt, &c = cmhashset_Hasher, &v]()
	{
		cout << "Ajy::cahash_multiset<int>,hash_function->Ajy::Hasher" << endl;
		cout << "{" << endl;
		StopWatch sw;
		for (int i = 0; i < int(v.size()); ++i)
		{
			c.emplace(v[i]);
		}
		for (int i = 0; i < int(v.size()); ++i)
		{
			c.emplace(i);
		}
		cout << "time cost(ms) for insert:   " << sw() << endl;
		cout << "size:   " << c.size() << endl;
		sw.Reset();
		long long val = 0;
		for (auto it = c.begin(); it != c.end(); ++it)
		{
			val += *it;
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
	auto teststd_Hasher = [&mtr, &mt, &c = stdhashset_Hasher, &v]()
	{
		cout << "std::unordered_multiset<int>,hash_function->Ajy::Hasher" << endl;
		cout << "{" << endl;
		StopWatch sw;
		for (int i = 0; i < int(v.size()); ++i)
		{
			c.emplace(v[i]);
		}
		for (int i = 0; i < int(v.size()); ++i)
		{
			c.emplace(i);
		}
		cout << "time cost(ms) for insert:   " << sw() << endl;
		cout << "size:   " << c.size() << endl;
		sw.Reset();
		long long val = 0;
		for (auto it = c.begin(); it != c.end(); ++it)
		{
			val += *it;
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

void HashMultiSet_Compare_2()
{
	std::mt19937 mt(0);
	auto mtr = std::uniform_int_distribution<int>(-10000, 10000);

	std::unordered_multiset<std::string> stdhashset_stdhash;
	cahash_multiset<std::string, std::hash<std::string>> cmhashset_stdhash;
	std::unordered_multiset<std::string, Hasher<std::string>> stdhashset_Hasher;
	cahash_multiset<std::string> cmhashset_Hasher;


	std::vector<int> v;
	v.resize(20000000);
	auto reset = [&mtr, &mt, &v]()
	{
		for (auto &value : v)
		{
			value = mtr(mt);
		}
	};

	auto testcm_stdhash = [&mtr, &mt, &c = cmhashset_stdhash, &v]()
	{
		cout << "Ajy::cahash_multiset<std::string>,hash_function->std::hash" << endl;
		cout << "{" << endl;
		StopWatch sw;
		for (int i = 0; i < int(v.size()); ++i)
		{
			char str[10];
			sprintf(str, "%d", v[i]);
			c.emplace(std::string(str));
		}
		for (int i = 0; i < int(v.size()); ++i)
		{
			char str[10];
			sprintf(str, "%d", i);
			c.emplace(std::string(str));
		}
		cout << "time cost(ms) for insert:   " << sw() << endl;
		cout << "size:   " << c.size() << endl;
		sw.Reset();
		int64 val = 0;
		int64 v = 0;
		for (auto it = c.begin(); it != c.end(); ++it)
		{
			Ajy::FromString(v, (*it).c_str());
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
	auto teststd_stdhash = [&mtr, &mt, &c = stdhashset_stdhash, &v]()
	{
		cout << "std::unordered_multiset<std::string>,hash_function->std::hash" << endl;
		cout << "{" << endl;
		StopWatch sw;
		for (int i = 0; i < int(v.size()); ++i)
		{
			char str[10];
			sprintf(str, "%d", v[i]);
			c.emplace(std::string(str));
		}
		for (int i = 0; i < int(v.size()); ++i)
		{
			char str[10];
			sprintf(str, "%d", i);
			c.emplace(std::string(str));
		}
		cout << "time cost(ms) for insert:   " << sw() << endl;
		cout << "size:   " << c.size() << endl;
		sw.Reset();
		int64 val = 0;
		int64 v = 0;
		for (auto it = c.begin(); it != c.end(); ++it)
		{
			Ajy::FromString(v, (*it).c_str());
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

	auto testcm_Hasher = [&mtr, &mt, &c = cmhashset_Hasher, &v]()
	{
		cout << "Ajy::cahash_multiset<std::string>,hash_function->Ajy::Hasher" << endl;
		cout << "{" << endl;
		StopWatch sw;
		for (int i = 0; i < int(v.size()); ++i)
		{
			char str[10];
			sprintf(str, "%d", v[i]);
			c.emplace(std::string(str));
		}
		for (int i = 0; i < int(v.size()); ++i)
		{
			char str[10];
			sprintf(str, "%d", i);
			c.emplace(std::string(str));
		}
		cout << "time cost(ms) for insert:   " << sw() << endl;
		cout << "size:   " << c.size() << endl;
		sw.Reset();
		int64 val = 0;
		int64 v = 0;
		for (auto it = c.begin(); it != c.end(); ++it)
		{
			Ajy::FromString(v, (*it).c_str());
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
	auto teststd_Hasher = [&mtr, &mt, &c = stdhashset_Hasher, &v]()
	{
		cout << "std::unordered_multiset<std::string>,hash_function->Ajy::Hasher" << endl;
		cout << "{" << endl;
		StopWatch sw;
		for (int i = 0; i < int(v.size()); ++i)
		{
			char str[10];
			sprintf(str, "%d", v[i]);
			c.emplace(std::string(str));
		}
		for (int i = 0; i < int(v.size()); ++i)
		{
			char str[10];
			sprintf(str, "%d", i);
			c.emplace(std::string(str));
		}
		cout << "time cost(ms) for insert:   " << sw() << endl;
		cout << "size:   " << c.size() << endl;
		sw.Reset();
		int64 val = 0;
		int64 v = 0;
		for (auto it = c.begin(); it != c.end(); ++it)
		{
			Ajy::FromString(v, (*it).c_str());
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

void HashMultiSet_Compare_3()
{
	std::mt19937 mt(0);
	auto mtr = std::uniform_int_distribution<int>(-10000, 10000);

	std::unordered_multiset<Ajy::String, Hasher<Ajy::String>> stdhashset_Hasher;
	cahash_multiset<Ajy::String> cmhashset_Hasher;


	std::vector<int> v;
	v.resize(1000000);
	auto reset = [&mtr, &mt, &v]()
	{
		for (auto &value : v)
		{
			value = mtr(mt);
		}
	};

	auto testcm_Hasher = [&mtr, &mt, &c = cmhashset_Hasher, &v]()
	{
		cout << "Ajy::cahash_multiset<Ajy::String>,hash_function->Ajy::Hasher" << endl;
		cout << "{" << endl;
		StopWatch sw;
		for (int i = 0; i < int(v.size()); ++i)
		{
			c.emplace(Ajy::String::MakeString(v[i]));
		}
		for (int i = 0; i < int(v.size()); ++i)
		{
			c.emplace(Ajy::String::MakeString(i));
		}
		cout << "time cost(ms) for insert:   " << sw() << endl;
		cout << "size:   " << c.size() << endl;
		sw.Reset();
		int64 val = 0;
		for (auto it = c.begin(); it != c.end(); ++it)
		{
			val += 1;
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
	auto teststd_Hasher = [&mtr, &mt, &c = stdhashset_Hasher, &v]()
	{
		cout << "std::unordered_multiset<Ajy::String>,hash_function->Ajy::Hasher" << endl;
		cout << "{" << endl;
		StopWatch sw;
		for (int i = 0; i < int(v.size()); ++i)
		{
			c.emplace(Ajy::String::MakeString(v[i]));
		}
		for (int i = 0; i < int(v.size()); ++i)
		{
			c.emplace(Ajy::String::MakeString(i));
		}
		cout << "time cost(ms) for insert:   " << sw() << endl;
		cout << "size:   " << c.size() << endl;
		sw.Reset();
		int64 val = 0;
		for (auto it = c.begin(); it != c.end(); ++it)
		{
			val += 1;
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

void HashMultiSet_Compare()
{
	//HashMultiSet_Compare_1();
	HashMultiSet_Compare_2();
	//HashMultiSet_Compare_3();
}
