#pragma once

void AVLTree_Test()
{

	avltree_map<int, int> avl_map;
	int a[] = { 16, 3, 7, 11, 9, 26, 18, 14, 15 };
	for (int i = 0; i < sizeof(a) / sizeof(a[0]); i++)
	{
		avl_map.emplace(a[i], a[i]);
	}
	cout << avl_map[33] << endl;
	cout << avl_map.at(16) << endl;
	cout << "----------iterator----------" << endl;
	for (auto it = avl_map.begin(); it != avl_map.end(); ++it)
	{
		cout << it->first << "   " << it->second << endl;
	}
	for (auto it = avl_map.begin(); it != avl_map.end();)
	{
		it = avl_map.erase(it);
	}
	cout << "----------reverse_iterator----------" << endl;
	for (auto it = avl_map.rbegin(); it != avl_map.rend(); ++it)
	{
		cout << it->first << "   " << it->second << endl;
	}
	cout << "size of avl_map:   " << avl_map.size() << endl;
	cout << "avl_map is balance?   " << avl_map.is_tree_balance() << endl;

	avltree_multimap<int, int, std::greater<int>> avl_multimap;
	int a1[] = { 16, 3, 7, 11, 16, 26, 18, 14, 15, 16, 3 };
	for (int i = 0; i < sizeof(a1) / sizeof(a1[0]); i++)
	{
		avl_multimap.emplace_hint(avl_multimap.begin(), a1[i], a1[i]);
	}
	cout << "----------iterator----------" << endl;
	for (auto it = avl_multimap.begin(); it != avl_multimap.end(); ++it)
	{
		cout << it->first << "   " << it->second << endl;
	}
	cout << "count of 16:   " << avl_multimap.count(16) <<endl;
	//avl_multimap.erase(16);
	auto pair_ii = avl_multimap.equal_range(16);
	for (auto it = pair_ii.first; it != pair_ii.second;)
	{
		it = avl_multimap.erase(it);
	}
	cout << "----------reverse_iterator----------" << endl;
	for (auto it = avl_multimap.rbegin(); it != avl_multimap.rend(); ++it)
	{
		cout << it->first << "   " << it->second << endl;
	}
	cout << "size of avl_multimap:   " << avl_multimap.size() << endl;
	cout << "avl_multimap is balance?   " << avl_multimap.is_tree_balance() << endl;

	avltree_set<int> avl_set;
	for (int i = 0; i < 20000000; ++i)
	{
		avl_set.emplace(i);
	}
	long long res = 0;
	for (int k : avl_set)
	{
		res += k;
	}
	cout << "res:  " << res << endl;
	res = 0;
	avltree_set<int> avl_set1(avl_set);
	for (int k : avl_set1)
	{
		res += k;
	}
	cout << "res:  " << res << endl;
	avltree_set<int> avl_set2;
	res = 0;
	avl_set2 = std::move(avl_set1);
	for (int k : avl_set1)
	{
		res += k;
	}
	cout << "res:  " << res << endl;
	for (int k : avl_set2)
	{
		res += k;
	}
	cout << "res:  " << res << endl;


	std::mt19937 mt(0);
	auto mtr = std::uniform_int_distribution<int>(-1000000, 1000000);

	std::multimap<int, int> stdmap_less;
	avltree_multimap<int, int> avltree_less;
	bpptree_multimap<int, int> bptree_less;


	std::vector<int> v;
	v.resize(20000000);
	auto reset = [&mtr, &mt, &v]()
	{
		for (auto &value : v)
		{
			value = mtr(mt);
		}
	};

	auto teststd_less = [&mtr, &mt, &c = stdmap_less, &v]()
	{
		cout << "std::multimap,std::less" << endl;
		cout << "{" << endl;
		StopWatch sw;
		for (int i = 0; i < int(v.size()); ++i)
		{
			//c.insert(std::make_pair(v[i], i));
			//c.emplace(v[i], i);
			c.insert(c.end(),std::make_pair(v[i], i));
		}
		for (int i = 0; i < int(v.size()); ++i)
		{
			//c.insert(std::make_pair(i, i));
			//c.emplace(i, i);
			c.insert(c.end(), std::make_pair(i, i));
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
		for (int i = 0; i < int(v.size()); ++i)
		{
			c.erase(v[i]);
		}
		for (int i = 0; i < int(v.size()); ++i)
		{
			c.erase(i);
		}
		cout << "time cost(ms) for erase:   " << sw() << endl;
		cout << "size:   " << c.size() << endl;
		cout << "}" << endl;
		c.clear();
	};
	auto testbp_less = [&mtr, &mt, &c = bptree_less, &v]()
	{
		cout << "Ajy::bpptree_multimap,std::less" << endl;
		cout << "{" << endl;
		StopWatch sw;
		for (int i = 0; i < int(v.size()); ++i)
		{
			//c.insert(std::make_pair(v[i], i));
			//c.emplace(v[i], i);
			c.insert(c.end(), std::make_pair(v[i], i));
		}
		for (int i = 0; i < int(v.size()); ++i)
		{
			//c.insert(std::make_pair(i, i));
			//c.emplace(i, i);
			c.insert(c.end(), std::make_pair(i, i));
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
		for (int i = 0; i < int(v.size()); ++i)
		{
			c.erase(v[i]);
		}
		for (int i = 0; i < int(v.size()); ++i)
		{
			c.erase(i);
		}
		cout << "time cost(ms) for erase:   " << sw() << endl;
		cout << "size:   " << c.size() << endl;
		cout << "}" << endl;
		c.clear();
	};
	auto testavl_less = [&mtr, &mt, &c = avltree_less, &v]()
	{
		cout << "avltree_multimap,std::less" << endl;
		cout << "{" << endl;
		StopWatch sw;
		for (int i = 0; i < int(v.size()); ++i)
		{
			//c.insert(std::make_pair(v[i], i));
			//c.emplace(v[i], i);
			c.insert(c.end(), std::make_pair(v[i], i));
		}
		for (int i = 0; i < int(v.size()); ++i)
		{
			//c.insert(std::make_pair(i, i));
			//c.emplace(i, i);
			c.insert(c.end(), std::make_pair(i, i));
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
		for (int i = 0; i < int(v.size()); ++i)
		{
			c.erase(v[i]);
		}
		for (int i = 0; i < int(v.size()); ++i)
		{
			c.erase(i);
		}
		/*for (auto it = c.begin(); it != c.end();)
		{
			it = c.erase(it);
		}*/
		cout << "time cost(ms) for erase:   " << sw() << endl;
		cout << "size:   " << c.size() << endl;
		cout << "}" << endl;
		c.clear();
	};

	reset();
	teststd_less();
	testbp_less();
	testavl_less();
}
