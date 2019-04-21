#pragma once

struct CMHashTestHelper
{
	int i;
	CMHashTestHelper(int _i) :i(_i) {}
	size_t GetHashCode() const
	{
		return (size_t)i;
	}
};
struct CMHashTestHelperDerive : CMHashTestHelper
{
	CMHashTestHelperDerive(int i) :CMHashTestHelper(i) {}
	/*size_t GetHashCode() const
	{
		return (size_t)i;
	}*/
};
struct CMHashTestHelperHasher
{
	size_t operator()(CMHashTestHelper const& o) const
	{
		return (size_t)o.i;
	}
};
inline bool operator==(CMHashTestHelper const& lhs, CMHashTestHelper const& rhs)
{
	return lhs.i == rhs.i;
}
size_t GetHashCode(CMHashTestHelperDerive const& in)
{
	return GetHashCode(in.i);
};

void CacheAffinityHash_Test()
{

	///************************************************************************/
	///* map_Test                                                             */
	///************************************************************************/

	//cahash_map<int, int> cm_map;
	//for (int i = 0; i < 10; ++i)
	//{
	//	cm_map.emplace(i, i);
	//}
	//cm_map.emplace(0, 0);
	//cm_map.erase(0);
	//cm_map.erase(++cm_map.begin());
	//cm_map.erase(cm_map.end());
	//for (auto o : cm_map)
	//{
	//	std::cout << o.first << "   " << o.second << std::endl;
	//}

	////cahash_map<CMHashTestHelper, int> cm_map1;		// hasher is use std::hash<CMHashTestHelper>
	//cahash_map<CMHashTestHelper, int, CMHashTestHelperHasher> cm_map1;	// hasher is use CMHashTestHelperHasher
	//for (int i = 0; i < 10; ++i)
	//{
	//	cm_map1.emplace(CMHashTestHelper(i), i);
	//}
	//cm_map1.emplace(CMHashTestHelper(0), 0);
	//cm_map1.erase(CMHashTestHelper(0));
	//cm_map1.erase(++cm_map1.begin());
	//cm_map1.erase(cm_map1.end());
	//for (auto o : cm_map1)
	//{
	//	std::cout << o.first.i << "   " << o.second << std::endl;
	//}

	///************************************************************************/
	///* multimap_Test                                                        */
	///************************************************************************/

	//cahash_multimap<int, int> cm_multimap;
	//for (int i = 0; i < 10; ++i)
	//{
	//	cm_multimap.emplace(i, i);
	//}
	//cm_multimap.emplace(0, 0);
	//cm_multimap.erase(0);
	//cm_multimap.erase(++cm_multimap.begin());
	//cm_multimap.erase(cm_multimap.end());
	//cm_multimap.emplace(0, 0);
	//cm_multimap.emplace(2, 2);
	//cm_multimap.emplace(0, 0);
	//for (auto o : cm_multimap)
	//{
	//	std::cout << o.first << "   " << o.second << std::endl;
	//}

	///************************************************************************/
	///* set_Test                                                             */
	///************************************************************************/

	//cahash_set<std::string> cm_set;
	//for (int i = 0; i < 10; ++i)
	//{
	//	char buf[10];
	//	sprintf(buf, "%d", i);
	//	cm_set.emplace(std::string(buf));
	//}
	//cm_set.erase(++cm_set.begin());
	//cm_set.erase(cm_set.end());
	//for (auto it = cm_set.begin(); it != cm_set.end(); ++it)
	//{
	//	std::cout << *it << std::endl;
	//}

	///************************************************************************/
	///* multiset_Test                                                        */
	///************************************************************************/

	//cahash_multiset<int> cm_multiset;
	//for (int i = 0; i < 10; ++i)
	//{
	//	cm_multiset.emplace(i);
	//}
	//cm_multiset.emplace(0);
	//cm_multiset.erase(0);
	//cm_multiset.erase(++cm_multiset.begin());
	//cm_multiset.erase(cm_multiset.end());
	//cm_multiset.emplace(0);
	//cm_multiset.emplace(2);
	//cm_multiset.emplace(0);
	//for (auto it = cm_multiset.begin(); it != cm_multiset.end(); ++it)
	//{
	//	std::cout << *it << std::endl;
	//}


	/************************************************************************************/
	/* Hasher Test															            */
	/* 主要测试Hasher的特化以及非特化情况下参数类型是否有GetHashCode成员函数下的匹配正确性	*/
	/************************************************************************************/

	cahash_map<int, int> cm_map_int_int;
	for (int i = -9; i < 10; ++i)
	{
		cm_map_int_int.emplace(i, i);
	}
	cm_map_int_int.erase(++cm_map_int_int.begin());
	cm_map_int_int.erase(cm_map_int_int.end());
	for (auto o : cm_map_int_int)
	{
		std::cout << o.first << "   " << o.second << std::endl;
	}

	cahash_map<std::string, int> cm_map_stdstr_int;
	for (int i = -9; i < 10; ++i)
	{
		char buf[5];
		sprintf(buf, "%d", i);
		cm_map_stdstr_int.emplace(std::string(buf), i);
	}
	cm_map_stdstr_int.erase(++cm_map_stdstr_int.begin());
	cm_map_stdstr_int.erase(cm_map_stdstr_int.end());
	for (auto o : cm_map_stdstr_int)
	{
		std::cout << o.first << "   " << o.second << std::endl;
	}

	cahash_map<CMHashTestHelper, int> cm_map_testhelper_int;
	for (int i = -9; i < 10; ++i)
	{
		cm_map_testhelper_int.emplace(CMHashTestHelper(i), i);
	}
	cm_map_testhelper_int.erase(++cm_map_testhelper_int.begin());
	cm_map_testhelper_int.erase(cm_map_testhelper_int.end());
	for (auto o : cm_map_testhelper_int)
	{
		std::cout << o.first.i << "   " << o.second << std::endl;
	}

	cahash_map<CMHashTestHelperDerive, int, Hasher<CMHashTestHelper>> cm_map_testhelperderive_int;
	for (int i = -9; i < 10; ++i)
	{
		cm_map_testhelperderive_int.emplace(CMHashTestHelperDerive(i), i);
	}
	cm_map_testhelperderive_int.erase(++cm_map_testhelperderive_int.begin());
	cm_map_testhelperderive_int.erase(cm_map_testhelperderive_int.end());
	for (auto o : cm_map_testhelperderive_int)
	{
		std::cout << o.first.i << "   " << o.second << std::endl;
	}

	cahash_multimap<int, int> cm_multimap;
	cm_multimap.emplace(5, 5);
	for (int i = 0; i < 10; ++i)
	{
		cm_multimap.emplace(i, i);
	}
	cm_multimap.emplace(5, 5);
	auto pair_lili = cm_multimap.equal_range(5);
	for (auto li = pair_lili.first; li != pair_lili.second; ++li)
	{
		Console::WriteLine((*li).first, "   ", li->second);
	}
	Cout.WriteLine(cm_multimap.count(5));

	/************************************************************************/
	/* hasher compare                                                       */
	/************************************************************************/
	/*std::cout << "cahash_map,std::hash<CMHashTestHelper>" << std::endl;
	std::cout << "{" << std::endl;
	{
		int n = 15000000;
		cahash_map<CMHashTestHelper, int,std::hash<CMHashTestHelper>> hash_map_compare;
		Ajy::StopWatch sw;
		for (int i = 0; i < n; ++i)
		{
			hash_map_compare.emplace(CMHashTestHelper(i), i);
		}
		std::cout << "time(ms) cost for insert:   " << sw() << endl;
		std::cout << "size:   " << hash_map_compare.size() << endl;
		long long c = 0;
		sw.Reset();
		for (auto it = hash_map_compare.begin(); it != hash_map_compare.end(); ++it)
		{
			c += (*it).second;
		}
		std::cout << "time(ms) cost for traverse:   " << sw() << endl;
		std::cout << c << std::endl;
		sw.Reset();
		for (int i = 0; i < n; ++i)
		{
			hash_map_compare.erase(CMHashTestHelper(i));
		}
		std::cout << "time(ms) cost for erase:   " << sw() << endl;
		std::cout << "size:   " << hash_map_compare.size() << endl;
	}
	std::cout << "}" << std::endl;
	std::cout << "cahash_map,CMHashTestHelperHasher" << std::endl;
	std::cout << "{" << std::endl;
	{
		int n = 15000000;
		cahash_map<CMHashTestHelper, int, CMHashTestHelperHasher> hash_map_compare;
		Ajy::StopWatch sw;
		for (int i = 0; i < n; ++i)
		{
			hash_map_compare.emplace(CMHashTestHelper(i), i);
		}
		std::cout << "time(ms) cost for insert:   " << sw() << endl;
		std::cout << "size:   " << hash_map_compare.size() << endl;
		long long c = 0;
		sw.Reset();
		for (auto it = hash_map_compare.begin(); it != hash_map_compare.end(); ++it)
		{
			c += (*it).second;
		}
		std::cout << "time(ms) cost for traverse:   " << sw() << endl;
		std::cout << c << std::endl;
		sw.Reset();
		for (int i = 0; i < n; ++i)
		{
			hash_map_compare.erase(CMHashTestHelper(i));
		}
		std::cout << "time(ms) cost for erase:   " << sw() << endl;
		std::cout << "size:   " << hash_map_compare.size() << endl;
	}
	std::cout << "}" << std::endl;
	std::cout << "std::unordered_map,std::hash<CMHashTestHelper>" << std::endl;
	std::cout << "{" << std::endl;
	{
		int n = 15000000;
		std::unordered_map<CMHashTestHelper, int> hash_map_compare;
		Ajy::StopWatch sw;
		for (int i = 0; i < n; ++i)
		{
			hash_map_compare.emplace(CMHashTestHelper(i), i);
		}
		std::cout << "time(ms) cost for insert:   " << sw() << endl;
		std::cout << "size:   " << hash_map_compare.size() << endl;
		long long c = 0;
		sw.Reset();
		for (auto it = hash_map_compare.begin(); it != hash_map_compare.end(); ++it)
		{
			c += (*it).second;
		}
		std::cout << "time(ms) cost for traverse:   " << sw() << endl;
		std::cout << c << std::endl;
		sw.Reset();
		for (int i = 0; i < n; ++i)
		{
			hash_map_compare.erase(CMHashTestHelper(i));
		}
		std::cout << "time(ms) cost for erase:   " << sw() << endl;
		std::cout << "size:   " << hash_map_compare.size() << endl;
	}
	std::cout << "}" << std::endl;
	std::cout << "std::unordered_map,CMHashTestHelperHasher" << std::endl;
	std::cout << "{" << std::endl;
	{
		int n = 15000000;
		std::unordered_map<CMHashTestHelper, int, CMHashTestHelperHasher> hash_map_compare;
		Ajy::StopWatch sw;
		for (int i = 0; i < n; ++i)
		{
			hash_map_compare.emplace(CMHashTestHelper(i), i);
		}
		std::cout << "time(ms) cost for insert:   " << sw() << endl;
		std::cout << "size:   " << hash_map_compare.size() << endl;
		long long c = 0;
		sw.Reset();
		for (auto it = hash_map_compare.begin(); it != hash_map_compare.end(); ++it)
		{
			c += (*it).second;
		}
		std::cout << "time(ms) cost for traverse:   " << sw() << endl;
		std::cout << c << std::endl;
		sw.Reset();
		for (int i = 0; i < n; ++i)
		{
			hash_map_compare.erase(CMHashTestHelper(i));
		}
		std::cout << "time(ms) cost for erase:   " << sw() << endl;
		std::cout << "size:   " << hash_map_compare.size() << endl;
	}
	std::cout << "}" << std::endl;*/



	/************************************************************************/
	/* hasher compare1                                                       */
	/************************************************************************/

	std::cout << "cahash_map,std::hash<int>" << std::endl;
	std::cout << "{" << std::endl;
	{
		int n = 15000000;
		cahash_map<int, int, std::hash<int>> hash_map_compare;
		Ajy::StopWatch sw;
		for (int i = -n + 1; i < n; ++i)
		{
			hash_map_compare.emplace(i, i);
		}
		std::cout << "time(ms) cost for insert:   " << sw() << endl;
		std::cout << "size:   " << hash_map_compare.size() << endl;
		long long c = 0;
		sw.Reset();
		for (auto it = hash_map_compare.begin(); it != hash_map_compare.end(); ++it)
		{
			c += (*it).second;
		}
		std::cout << "time(ms) cost for traverse:   " << sw() << endl;
		std::cout << c << std::endl;
		c = 0;
		sw.Reset();
		for (int i = -n + 1; i < n; ++i)
		{
			c += (*hash_map_compare.find(i)).second;
		}
		std::cout << "time(ms) cost for find:   " << sw() << endl;
		std::cout << c << std::endl;
		sw.Reset();
		for (auto it = hash_map_compare.begin(); it != hash_map_compare.end();)
		{
			it = hash_map_compare.erase(it);
		}
		std::cout << "time(ms) cost for erase:   " << sw() << endl;
		std::cout << "size:   " << hash_map_compare.size() << endl;
	}
	std::cout << "}" << std::endl;
	std::cout << "cahash_map,Hasher<int>" << std::endl;
	std::cout << "{" << std::endl;
	{
		int n = 15000000;
		cahash_map<int, int> hash_map_compare;
		Ajy::StopWatch sw;
		for (int i = -n + 1; i < n; ++i)
		{
			hash_map_compare.emplace(i, i);
		}
		std::cout << "time(ms) cost for insert:   " << sw() << endl;
		std::cout << "size:   " << hash_map_compare.size() << endl;
		long long c = 0;
		sw.Reset();
		for (auto it = hash_map_compare.begin(); it != hash_map_compare.end(); ++it)
		{
			c += (*it).second;
		}
		std::cout << "time(ms) cost for traverse:   " << sw() << endl;
		std::cout << c << std::endl;
		c = 0;
		sw.Reset();
		for (int i = -n + 1; i < n; ++i)
		{
			c += (*hash_map_compare.find(i)).second;
		}
		std::cout << "time(ms) cost for find:   " << sw() << endl;
		std::cout << c << std::endl;
		sw.Reset();
		for (auto it = hash_map_compare.begin(); it != hash_map_compare.end();)
		{
			it = hash_map_compare.erase(it);
		}
		std::cout << "time(ms) cost for erase:   " << sw() << endl;
		std::cout << "size:   " << hash_map_compare.size() << endl;
	}
	std::cout << "}" << std::endl;
	std::cout << "std::unordered_map,std::hash<int>" << std::endl;
	std::cout << "{" << std::endl;
	{
		int n = 15000000;
		std::unordered_map<int, int> hash_map_compare;
		Ajy::StopWatch sw;
		for (int i = -n + 1; i < n; ++i)
		{
			hash_map_compare.emplace(i, i);
		}
		std::cout << "time(ms) cost for insert:   " << sw() << endl;
		std::cout << "size:   " << hash_map_compare.size() << endl;
		long long c = 0;
		sw.Reset();
		for (auto it = hash_map_compare.begin(); it != hash_map_compare.end(); ++it)
		{
			c += (*it).second;
		}
		std::cout << "time(ms) cost for traverse:   " << sw() << endl;
		std::cout << c << std::endl;
		c = 0;
		sw.Reset();
		for (int i = -n + 1; i < n; ++i)
		{
			c += (*hash_map_compare.find(i)).second;
		}
		std::cout << "time(ms) cost for find:   " << sw() << endl;
		std::cout << c << std::endl;
		sw.Reset();
		for (auto it = hash_map_compare.begin(); it != hash_map_compare.end();)
		{
			it = hash_map_compare.erase(it);
		}
		std::cout << "time(ms) cost for erase:   " << sw() << endl;
		std::cout << "size:   " << hash_map_compare.size() << endl;
	}
	std::cout << "}" << std::endl;
	std::cout << "std::unordered_map,Hasher<int>" << std::endl;
	std::cout << "{" << std::endl;
	{
		int n = 15000000;
		std::unordered_map<int, int, Hasher<int>> hash_map_compare;
		Ajy::StopWatch sw;
		for (int i = -n + 1; i < n; ++i)
		{
			hash_map_compare.emplace(i, i);
		}
		std::cout << "time(ms) cost for insert:   " << sw() << endl;
		std::cout << "size:   " << hash_map_compare.size() << endl;
		long long c = 0;
		sw.Reset();
		for (auto it = hash_map_compare.begin(); it != hash_map_compare.end(); ++it)
		{
			c += (*it).second;
		}
		std::cout << "time(ms) cost for traverse:   " << sw() << endl;
		std::cout << c << std::endl;
		c = 0;
		sw.Reset();
		for (int i = -n + 1; i < n; ++i)
		{
			c += (*hash_map_compare.find(i)).second;
		}
		std::cout << "time(ms) cost for find:   " << sw() << endl;
		std::cout << c << std::endl;
		sw.Reset();
		for (auto it = hash_map_compare.begin(); it != hash_map_compare.end();)
		{
			it = hash_map_compare.erase(it);
		}
		std::cout << "time(ms) cost for erase:   " << sw() << endl;
		std::cout << "size:   " << hash_map_compare.size() << endl;
	}
	std::cout << "}" << std::endl;



	/************************************************************************/
	/* hasher compare2                                                       */
	/************************************************************************/

	//// 经测试，对于std::string类型，stl使用std::hash插入性能比Hasher好,但查找和删除性能差

	std::cout << "cahash_map,std::hash<std::string>" << std::endl;
	std::cout << "{" << std::endl;
	{
		int n = 15000000;
		cahash_map<std::string, int, std::hash<std::string>> hash_map_compare;
		Ajy::StopWatch sw;
		for (int i = -n + 1; i < n; ++i)
		{
			char buf[10];
			sprintf(buf, "%d", i);
			hash_map_compare.emplace(std::string(buf), i);
		}
		std::cout << "time(ms) cost for insert:   " << sw() << endl;
		std::cout << "size:   " << hash_map_compare.size() << endl;
		long long c = 0;
		sw.Reset();
		for (auto it = hash_map_compare.begin(); it != hash_map_compare.end(); ++it)
		{
			c += (*it).second;
		}
		std::cout << "time(ms) cost for traverse:   " << sw() << endl;
		std::cout << c << std::endl;
		c = 0;
		sw.Reset();
		for (int i = -n + 1; i < n; ++i)
		{
			char buf[10];
			sprintf(buf, "%d", i);
			c += (*hash_map_compare.find(std::string(buf))).second;
		}
		std::cout << "time(ms) cost for find:   " << sw() << endl;
		std::cout << c << std::endl;
		sw.Reset();
		for (auto it = hash_map_compare.begin(); it != hash_map_compare.end();)
		{
			it = hash_map_compare.erase(it);
		}
		std::cout << "time(ms) cost for erase:   " << sw() << endl;
		std::cout << "size:   " << hash_map_compare.size() << endl;
	}
	std::cout << "}" << std::endl;
	std::cout << "cahash_map,Hasher<std::string>" << std::endl;
	std::cout << "{" << std::endl;
	{
		int n = 15000000;
		cahash_map<std::string, int> hash_map_compare;
		Ajy::StopWatch sw;
		for (int i = -n + 1; i < n; ++i)
		{
			char buf[10];
			sprintf(buf, "%d", i);
			hash_map_compare.emplace(std::string(buf), i);
		}
		std::cout << "time(ms) cost for insert:   " << sw() << endl;
		std::cout << "size:   " << hash_map_compare.size() << endl;
		long long c = 0;
		sw.Reset();
		for (auto it = hash_map_compare.begin(); it != hash_map_compare.end(); ++it)
		{
			c += (*it).second;
		}
		std::cout << "time(ms) cost for traverse:   " << sw() << endl;
		std::cout << c << std::endl;
		c = 0;
		sw.Reset();
		for (int i = -n + 1; i < n; ++i)
		{
			char buf[10];
			sprintf(buf, "%d", i);
			c += (*hash_map_compare.find(std::string(buf))).second;
		}
		std::cout << "time(ms) cost for find:   " << sw() << endl;
		std::cout << c << std::endl;
		sw.Reset();
		for (auto it = hash_map_compare.begin(); it != hash_map_compare.end();)
		{
			it = hash_map_compare.erase(it);
		}
		std::cout << "time(ms) cost for erase:   " << sw() << endl;
		std::cout << "size:   " << hash_map_compare.size() << endl;
	}
	std::cout << "}" << std::endl;
	std::cout << "std::unordered_map,std::hash<std::string>" << std::endl;
	std::cout << "{" << std::endl;
	{
		int n = 15000000;
		std::unordered_map<std::string, int> hash_map_compare;
		Ajy::StopWatch sw;
		for (int i = -n + 1; i < n; ++i)
		{
			char buf[10];
			sprintf(buf, "%d", i);
			hash_map_compare.emplace(std::string(buf), i);
		}
		std::cout << "time(ms) cost for insert:   " << sw() << endl;
		std::cout << "size:   " << hash_map_compare.size() << endl;
		long long c = 0;
		sw.Reset();
		for (auto it = hash_map_compare.begin(); it != hash_map_compare.end(); ++it)
		{
			c += (*it).second;
		}
		std::cout << "time(ms) cost for traverse:   " << sw() << endl;
		std::cout << c << std::endl;
		c = 0;
		sw.Reset();
		for (int i = -n + 1; i < n; ++i)
		{
			char buf[10];
			sprintf(buf, "%d", i);
			c += (*hash_map_compare.find(std::string(buf))).second;
		}
		std::cout << "time(ms) cost for find:   " << sw() << endl;
		std::cout << c << std::endl;
		sw.Reset();
		for (auto it = hash_map_compare.begin(); it != hash_map_compare.end();)
		{
			it = hash_map_compare.erase(it);
		}
		std::cout << "time(ms) cost for erase:   " << sw() << endl;
		std::cout << "size:   " << hash_map_compare.size() << endl;
	}
	std::cout << "}" << std::endl;
	std::cout << "std::unordered_map,Hasher<std::string>" << std::endl;
	std::cout << "{" << std::endl;
	{
		int n = 15000000;
		std::unordered_map<std::string, int, Hasher<std::string>> hash_map_compare;
		Ajy::StopWatch sw;
		for (int i = -n + 1; i < n; ++i)
		{
			char buf[10];
			sprintf(buf, "%d", i);
			hash_map_compare.emplace(std::string(buf), i);
		}
		std::cout << "time(ms) cost for insert:   " << sw() << endl;
		std::cout << "size:   " << hash_map_compare.size() << endl;
		long long c = 0;
		sw.Reset();
		for (auto it = hash_map_compare.begin(); it != hash_map_compare.end(); ++it)
		{
			c += (*it).second;
		}
		std::cout << "time(ms) cost for traverse:   " << sw() << endl;
		std::cout << c << std::endl;
		c = 0;
		sw.Reset();
		for (int i = -n + 1; i < n; ++i)
		{
			char buf[10];
			sprintf(buf, "%d", i);
			c += (*hash_map_compare.find(std::string(buf))).second;
		}
		std::cout << "time(ms) cost for find:   " << sw() << endl;
		std::cout << c << std::endl;
		sw.Reset();
		for (auto it = hash_map_compare.begin(); it != hash_map_compare.end();)
		{
			it = hash_map_compare.erase(it);
		}
		std::cout << "time(ms) cost for erase:   " << sw() << endl;
		std::cout << "size:   " << hash_map_compare.size() << endl;
	}
	std::cout << "}" << std::endl;


	/************************************************************************/
	/* hasher compare3                                                      */
	/************************************************************************/

	std::cout << "cahash_map,std::hash<double>" << std::endl;
	std::cout << "{" << std::endl;
	{
		int n = 15000000;
		cahash_map<double, double, std::hash<double>> hash_map_compare;
		Ajy::StopWatch sw;
		for (int i = -n + 1; i < n; ++i)
		{
			hash_map_compare.emplace(i + 0.123, i + 0.123);
		}
		std::cout << "time(ms) cost for insert:   " << sw() << endl;
		std::cout << "size:   " << hash_map_compare.size() << endl;
		double c = 0;
		sw.Reset();
		for (auto it = hash_map_compare.begin(); it != hash_map_compare.end(); ++it)
		{
			c += (*it).second;
		}
		std::cout << "time(ms) cost for traverse:   " << sw() << endl;
		std::cout << c << std::endl;
		c = 0;
		sw.Reset();
		for (int i = -n + 1; i < n; ++i)
		{
			c += (*hash_map_compare.find(i + 0.123)).second;
		}
		std::cout << "time(ms) cost for find:   " << sw() << endl;
		std::cout << c << std::endl;
		sw.Reset();
		for (auto it = hash_map_compare.begin(); it != hash_map_compare.end();)
		{
			it = hash_map_compare.erase(it);
		}
		std::cout << "time(ms) cost for erase:   " << sw() << endl;
		std::cout << "size:   " << hash_map_compare.size() << endl;
	}
	std::cout << "}" << std::endl;
	std::cout << "cahash_map,Hasher<double>" << std::endl;
	std::cout << "{" << std::endl;
	{
		int n = 15000000;
		cahash_map<double, double> hash_map_compare;
		Ajy::StopWatch sw;
		for (int i = -n + 1; i < n; ++i)
		{
			hash_map_compare.emplace(i + 0.123, i + 0.123);
		}
		std::cout << "time(ms) cost for insert:   " << sw() << endl;
		std::cout << "size:   " << hash_map_compare.size() << endl;
		double c = 0;
		sw.Reset();
		for (auto it = hash_map_compare.begin(); it != hash_map_compare.end(); ++it)
		{
			c += (*it).second;
		}
		std::cout << "time(ms) cost for traverse:   " << sw() << endl;
		std::cout << c << std::endl;
		c = 0;
		sw.Reset();
		for (int i = -n + 1; i < n; ++i)
		{
			c += (*hash_map_compare.find(i + 0.123)).second;
		}
		std::cout << "time(ms) cost for find:   " << sw() << endl;
		std::cout << c << std::endl;
		sw.Reset();
		for (auto it = hash_map_compare.begin(); it != hash_map_compare.end();)
		{
			it = hash_map_compare.erase(it);
		}
		std::cout << "time(ms) cost for erase:   " << sw() << endl;
		std::cout << "size:   " << hash_map_compare.size() << endl;
	}
	std::cout << "}" << std::endl;
	std::cout << "std::unordered_map,std::hash<double>" << std::endl;
	std::cout << "{" << std::endl;
	{
		int n = 15000000;
		std::unordered_map<double, double> hash_map_compare;
		Ajy::StopWatch sw;
		for (int i = -n + 1; i < n; ++i)
		{
			hash_map_compare.emplace(i + 0.123, i + 0.123);
		}
		std::cout << "time(ms) cost for insert:   " << sw() << endl;
		std::cout << "size:   " << hash_map_compare.size() << endl;
		double c = 0;
		sw.Reset();
		for (auto it = hash_map_compare.begin(); it != hash_map_compare.end(); ++it)
		{
			c += (*it).second;
		}
		std::cout << "time(ms) cost for traverse:   " << sw() << endl;
		std::cout << c << std::endl;
		c = 0;
		sw.Reset();
		for (int i = -n + 1; i < n; ++i)
		{
			c += (*hash_map_compare.find(i + 0.123)).second;
		}
		std::cout << "time(ms) cost for find:   " << sw() << endl;
		std::cout << c << std::endl;
		sw.Reset();
		for (auto it = hash_map_compare.begin(); it != hash_map_compare.end();)
		{
			it = hash_map_compare.erase(it);
		}
		std::cout << "time(ms) cost for erase:   " << sw() << endl;
		std::cout << "size:   " << hash_map_compare.size() << endl;
	}
	std::cout << "}" << std::endl;
	std::cout << "std::unordered_map,Hasher<double>" << std::endl;
	std::cout << "{" << std::endl;
	{
		int n = 15000000;
		std::unordered_map<double, double, Hasher<double>> hash_map_compare;
		Ajy::StopWatch sw;
		for (int i = -n + 1; i < n; ++i)
		{
			hash_map_compare.emplace(i + 0.123, i + 0.123);
		}
		std::cout << "time(ms) cost for insert:   " << sw() << endl;
		std::cout << "size:   " << hash_map_compare.size() << endl;
		double c = 0;
		sw.Reset();
		for (auto it = hash_map_compare.begin(); it != hash_map_compare.end(); ++it)
		{
			c += (*it).second;
		}
		std::cout << "time(ms) cost for traverse:   " << sw() << endl;
		std::cout << c << std::endl;
		c = 0;
		sw.Reset();
		for (int i = -n + 1; i < n; ++i)
		{
			c += (*hash_map_compare.find(i + 0.123)).second;
		}
		std::cout << "time(ms) cost for find:   " << sw() << endl;
		std::cout << c << std::endl;
		sw.Reset();
		for (auto it = hash_map_compare.begin(); it != hash_map_compare.end();)
		{
			it = hash_map_compare.erase(it);
		}
		std::cout << "time(ms) cost for erase:   " << sw() << endl;
		std::cout << "size:   " << hash_map_compare.size() << endl;
	}
	std::cout << "}" << std::endl;

}

