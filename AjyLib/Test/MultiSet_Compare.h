#pragma once

void MultiSet_Compare_1()
{
	std::mt19937 mt(0);
	auto mtr = std::uniform_int_distribution<int>(-1000000, 1000000);

	std::multiset<int> stdset_less;
	bpptree_multiset<int> bptree_less;
	sbtree_multiset<int> sbtree_less;
	rbtree_multiset<int> redblacktree_less;
	avltree_multiset<int> avltree_less;
	MultiSet<int> rbtree_less;
	std::multiset<int, std::greater<int>> stdset_great;
	bpptree_multiset<int, std::greater<int>> bptree_great;
	sbtree_multiset<int, std::greater<int>> sbtree_great;
	rbtree_multiset<int, std::greater<int>> redblacktree_great;
	avltree_multiset<int, std::greater<int>> avltree_great;
	MultiSet<int, std::greater<int>> rbtree_great;


	std::vector<int> v;
	v.resize(20000000);
	auto reset = [&mtr, &mt, &v]()
	{
		for (auto &value : v)
		{
			value = mtr(mt);
		}
	};

	auto testbp_less = [&mtr, &mt, &c = bptree_less, &v]()
	{
		cout << "bpptree_multiset,std::less" << endl;
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
		val = 0;
		sw.Reset();
		for (int i = 0; i < int(v.size()); ++i)
		{
			if (c.find(v[i]) != c.end())
			{
				val += 1;
			}
		}
		for (int i = 0; i < int(v.size()); ++i)
		{
			if (c.find(v[i]) != c.end())
			{
				val += 1;
			}
		}
		cout << "time cost(ms) for find:   " << sw() << endl;
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
	auto testredblack_less = [&mtr, &mt, &c = redblacktree_less, &v]()
	{
		cout << "rbtree_multiset,std::less" << endl;
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
		val = 0;
		sw.Reset();
		for (int i = 0; i < int(v.size()); ++i)
		{
			if (c.find(v[i]) != c.end())
			{
				val += 1;
			}
		}
		for (int i = 0; i < int(v.size()); ++i)
		{
			if (c.find(v[i]) != c.end())
			{
				val += 1;
			}
		}
		cout << "time cost(ms) for find:   " << sw() << endl;
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
	auto teststd_less = [&mtr, &mt, &c = stdset_less, &v]()
	{
		cout << "std::multiset,std::less" << endl;
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
		val = 0;
		sw.Reset();
		for (int i = 0; i < int(v.size()); ++i)
		{
			if (c.find(v[i]) != c.end())
			{
				val += 1;
			}
		}
		for (int i = 0; i < int(v.size()); ++i)
		{
			if (c.find(v[i]) != c.end())
			{
				val += 1;
			}
		}
		cout << "time cost(ms) for find:   " << sw() << endl;
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
	auto testsb_less = [&mtr, &mt, &c = sbtree_less, &v]()
	{
		cout << "sbtree_multiset,std::less" << endl;
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
		val = 0;
		sw.Reset();
		for (int i = 0; i < int(v.size()); ++i)
		{
			if (c.find(v[i]) != c.end())
			{
				val += 1;
			}
		}
		for (int i = 0; i < int(v.size()); ++i)
		{
			if (c.find(v[i]) != c.end())
			{
				val += 1;
			}
		}
		cout << "time cost(ms) for find:   " << sw() << endl;
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
	auto testrbtree_less = [&mtr, &mt, &c = rbtree_less, &v]()
	{
		cout << "Ajy::Multiset,std::less" << endl;
		cout << "{" << endl;
		StopWatch sw;
		for (int i = 0; i < int(v.size()); ++i)
		{
			c.Insert(v[i]);
		}
		for (int i = 0; i < int(v.size()); ++i)
		{
			c.Insert(i);
		}
		cout << "time cost(ms) for insert:   " << sw() << endl;
		cout << "size:   " << c.Size() << endl;
		sw.Reset();
		long long val = 0;
		for (auto it = c.Begin(); it != c.End(); ++it)
		{
			val += *it;
		}
		cout << "time cost(ms) for traverse:   " << sw() << endl;
		cout << "val:   " << val << endl;
		val = 0;
		sw.Reset();
		for (int i = 0; i < int(v.size()); ++i)
		{
			if (c.Find(v[i]) != c.End())
			{
				val += 1;
			}
		}
		for (int i = 0; i < int(v.size()); ++i)
		{
			if (c.Find(i) != c.End())
			{
				val += 1;
			}
		}
		cout << "time cost(ms) for find:   " << sw() << endl;
		cout << "val:   " << val << endl;
		sw.Reset();
		for (auto it = c.Begin(); it != c.End();)
		{
			it = c.Delete(it);
		}
		/*for (int i = 0; i < int(v.size()); ++i)
		{
		c.Delete(v[i]);
		}
		for (int i = 0; i < int(v.size()); ++i)
		{
		c.Delete(i);
		}*/
		cout << "time cost(ms) for erase:   " << sw() << endl;
		cout << "size:   " << c.Size() << endl;
		cout << "}" << endl;
		c.Clear();
	};
	auto testavl_less = [&mtr, &mt, &c = avltree_less, &v]()
	{
		cout << "Ajy::avltree_multiset,std::less" << endl;
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
		val = 0;
		sw.Reset();
		for (int i = 0; i < int(v.size()); ++i)
		{
			if (c.find(v[i]) != c.end())
			{
				val += 1;
			}
		}
		for (int i = 0; i < int(v.size()); ++i)
		{
			if (c.find(v[i]) != c.end())
			{
				val += 1;
			}
		}
		cout << "time cost(ms) for find:   " << sw() << endl;
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



	auto testbp_great = [&mtr, &mt, &c = bptree_great, &v]()
	{
		cout << "bpptree_multiset,std::greater" << endl;
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
		val = 0;
		sw.Reset();
		for (int i = 0; i < int(v.size()); ++i)
		{
			if (c.find(v[i]) != c.end())
			{
				val += 1;
			}
		}
		for (int i = 0; i < int(v.size()); ++i)
		{
			if (c.find(v[i]) != c.end())
			{
				val += 1;
			}
		}
		cout << "time cost(ms) for find:   " << sw() << endl;
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
	auto testredblack_great = [&mtr, &mt, &c = redblacktree_great, &v]()
	{
		cout << "rbtree_multiset,std::greater" << endl;
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
		val = 0;
		sw.Reset();
		for (int i = 0; i < int(v.size()); ++i)
		{
			if (c.find(v[i]) != c.end())
			{
				val += 1;
			}
		}
		for (int i = 0; i < int(v.size()); ++i)
		{
			if (c.find(v[i]) != c.end())
			{
				val += 1;
			}
		}
		cout << "time cost(ms) for find:   " << sw() << endl;
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
	auto teststd_great = [&mtr, &mt, &c = stdset_great, &v]()
	{
		cout << "std::multiset,std::greater" << endl;
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
		val = 0;
		sw.Reset();
		for (int i = 0; i < int(v.size()); ++i)
		{
			if (c.find(v[i]) != c.end())
			{
				val += 1;
			}
		}
		for (int i = 0; i < int(v.size()); ++i)
		{
			if (c.find(v[i]) != c.end())
			{
				val += 1;
			}
		}
		cout << "time cost(ms) for find:   " << sw() << endl;
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
	auto testsb_great = [&mtr, &mt, &c = sbtree_great, &v]()
	{
		cout << "sbtree_multiset,std::greater" << endl;
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
		val = 0;
		sw.Reset();
		for (int i = 0; i < int(v.size()); ++i)
		{
			if (c.find(v[i]) != c.end())
			{
				val += 1;
			}
		}
		for (int i = 0; i < int(v.size()); ++i)
		{
			if (c.find(v[i]) != c.end())
			{
				val += 1;
			}
		}
		cout << "time cost(ms) for find:   " << sw() << endl;
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
	auto testrbtree_great = [&mtr, &mt, &c = rbtree_great, &v]()
	{
		cout << "Ajy::Multiset,std::greater" << endl;
		cout << "{" << endl;
		StopWatch sw;
		for (int i = 0; i < int(v.size()); ++i)
		{
			c.Insert(v[i]);
		}
		for (int i = 0; i < int(v.size()); ++i)
		{
			c.Insert(i);
		}
		cout << "time cost(ms) for insert:   " << sw() << endl;
		cout << "size:   " << c.Size() << endl;
		sw.Reset();
		long long val = 0;
		for (auto it = c.Begin(); it != c.End(); ++it)
		{
			val += *it;
		}
		cout << "time cost(ms) for traverse:   " << sw() << endl;
		cout << "val:   " << val << endl;
		val = 0;
		sw.Reset();
		for (int i = 0; i < int(v.size()); ++i)
		{
			if (c.Find(v[i]) != c.End())
			{
				val += 1;
			}
		}
		for (int i = 0; i < int(v.size()); ++i)
		{
			if (c.Find(i) != c.End())
			{
				val += 1;
			}
		}
		cout << "time cost(ms) for find:   " << sw() << endl;
		cout << "val:   " << val << endl;
		sw.Reset();
		for (auto it = c.Begin(); it != c.End();)
		{
			it = c.Delete(it);
		}
		/*for (int i = 0; i < int(v.size()); ++i)
		{
		c.Delete(v[i]);
		}
		for (int i = 0; i < int(v.size()); ++i)
		{
		c.Delete(i);
		}*/
		cout << "time cost(ms) for erase:   " << sw() << endl;
		cout << "size:   " << c.Size() << endl;
		cout << "}" << endl;
		c.Clear();
	};
	auto testavl_great = [&mtr, &mt, &c = avltree_great, &v]()
	{
		cout << "Ajy::avltree_multiset,std::greater" << endl;
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
		val = 0;
		sw.Reset();
		for (int i = 0; i < int(v.size()); ++i)
		{
			if (c.find(v[i]) != c.end())
			{
				val += 1;
			}
		}
		for (int i = 0; i < int(v.size()); ++i)
		{
			if (c.find(v[i]) != c.end())
			{
				val += 1;
			}
		}
		cout << "time cost(ms) for find:   " << sw() << endl;
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
	testbp_less();
	testredblack_less();
	testavl_less();
	teststd_less();
	testrbtree_less();
	testsb_less();
	reset();
	testbp_less();
	testredblack_less();
	testavl_less();
	teststd_less();
	testrbtree_less();
	testsb_less();
	reset();
	testbp_less();
	testredblack_less();
	testavl_less();
	teststd_less();
	testrbtree_less();
	testsb_less();

	cout << "=====================================" << endl;

	reset();
	testbp_great();
	testredblack_great();
	testavl_great();
	teststd_great();
	testrbtree_great();
	testsb_great();
	reset();
	testbp_great();
	testredblack_great();
	testavl_great();
	teststd_great();
	testrbtree_great();
	testsb_great();
	reset();
	testbp_great();
	testredblack_great();
	testavl_great();
	teststd_great();
	testrbtree_great();
	testsb_great();

	v.clear();
}

void MultiSet_Compare()
{
	MultiSet_Compare_1();
}
