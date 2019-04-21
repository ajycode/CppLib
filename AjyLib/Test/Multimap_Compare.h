#pragma once

void Multimap_Compare()
{

	/************************************************************************/
	/* test1                                                                */
	/************************************************************************/

	std::mt19937 mt(0);
	auto mtr = std::uniform_int_distribution<int>(-1000000, 1000000);

	std::multimap<int, int> stdmap_less;
	Ajy::bpptree_multimap<int, int> bptree_less;
	Ajy::sbtree_multimap<int, int> sbtree_less;
	Ajy::rbtree_multimap<int, int> redblacktree_less;
	Ajy::avltree_multimap<int, int> avltree_less;
	std::multimap<int, int, std::greater<int>> stdmap_great;
	Ajy::bpptree_multimap<int, int, std::greater<int>> bptree_great;
	Ajy::sbtree_multimap<int, int, std::greater<int>> sbtree_great;
	Ajy::rbtree_multimap<int, int, std::greater<int>> redblacktree_great;
	Ajy::avltree_multimap<int, int, std::greater<int>> avltree_great;


	std::vector<int> v;
	v.resize(20000000);
	auto reset = [&mtr, &mt, &v]()
	{
		for (auto& value : v)
		{
			value = mtr(mt);
		}
	};

	auto testbp_less = [&mtr, &mt, &c = bptree_less, &v]()
	{
		cout << "bpptree_multimap,std::less" << endl;
		cout << "{" << endl;
		StopWatch sw;
		for (int i = 0; i < int(v.size()); ++i)
		{
			c.insert(std::make_pair(v[i], v[i]));
		}
		for (int i = 0; i < int(v.size()); ++i)
		{
			c.insert(std::make_pair(i, v[i]));
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
		val = 0;
		sw.Reset();
		for (int i = 0; i < int(v.size()); ++i)
		{
			auto ii = c.equal_range(v[i]);
			for (auto it = ii.first; it != ii.second; ++it)
			{
				val += it->second;
			}
		}
		for (int i = 0; i < int(v.size()); ++i)
		{
			auto ii = c.equal_range(i);
			for (auto it = ii.first; it != ii.second; ++it)
			{
				val += it->second;
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
		cout << "rbtree_multimap,std::less" << endl;
		cout << "{" << endl;
		StopWatch sw;
		for (int i = 0; i < int(v.size()); ++i)
		{
			c.insert(std::make_pair(v[i], v[i]));
		}
		for (int i = 0; i < int(v.size()); ++i)
		{
			c.insert(std::make_pair(i, v[i]));
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
		val = 0;
		sw.Reset();
		for (int i = 0; i < int(v.size()); ++i)
		{
			auto ii = c.equal_range(v[i]);
			for (auto it = ii.first; it != ii.second; ++it)
			{
				val += it->second;
			}
		}
		for (int i = 0; i < int(v.size()); ++i)
		{
			auto ii = c.equal_range(i);
			for (auto it = ii.first; it != ii.second; ++it)
			{
				val += it->second;
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
	auto teststd_less = [&mtr, &mt, &c = stdmap_less, &v]()
	{
		cout << "std::multimap,std::less" << endl;
		cout << "{" << endl;
		StopWatch sw;
		for (int i = 0; i < int(v.size()); ++i)
		{
			c.insert(std::make_pair(v[i], v[i]));
		}
		for (int i = 0; i < int(v.size()); ++i)
		{
			c.insert(std::make_pair(i, v[i]));
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
		val = 0;
		sw.Reset();
		for (int i = 0; i < int(v.size()); ++i)
		{
			auto ii = c.equal_range(v[i]);
			for (auto it = ii.first; it != ii.second; ++it)
			{
				val += it->second;
			}
		}
		for (int i = 0; i < int(v.size()); ++i)
		{
			auto ii = c.equal_range(i);
			for (auto it = ii.first; it != ii.second; ++it)
			{
				val += it->second;
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
		cout << "sbtree_multimap,std::less" << endl;
		cout << "{" << endl;
		StopWatch sw;
		for (int i = 0; i < int(v.size()); ++i)
		{
			c.insert(std::make_pair(v[i], v[i]));
		}
		for (int i = 0; i < int(v.size()); ++i)
		{
			c.insert(std::make_pair(i, v[i]));
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
		val = 0;
		sw.Reset();
		for (int i = 0; i < int(v.size()); ++i)
		{
			auto ii = c.equal_range(v[i]);
			for (auto it = ii.first; it != ii.second; ++it)
			{
				val += it->second;
			}
		}
		for (int i = 0; i < int(v.size()); ++i)
		{
			auto ii = c.equal_range(i);
			for (auto it = ii.first; it != ii.second; ++it)
			{
				val += it->second;
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
	auto testavl_less = [&mtr, &mt, &c = avltree_less, &v]()
	{
		cout << "Ajy::avltree_multimap,std::less" << endl;
		cout << "{" << endl;
		StopWatch sw;
		for (int i = 0; i < int(v.size()); ++i)
		{
			c.insert(std::make_pair(v[i], v[i]));
		}
		for (int i = 0; i < int(v.size()); ++i)
		{
			c.insert(std::make_pair(i, v[i]));
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
		val = 0;
		sw.Reset();
		for (int i = 0; i < int(v.size()); ++i)
		{
			auto ii = c.equal_range(v[i]);
			for (auto it = ii.first; it != ii.second; ++it)
			{
				val += it->second;
			}
		}
		for (int i = 0; i < int(v.size()); ++i)
		{
			auto ii = c.equal_range(i);
			for (auto it = ii.first; it != ii.second; ++it)
			{
				val += it->second;
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
		cout << "bpptree_multimap,std::greater" << endl;
		cout << "{" << endl;
		StopWatch sw;
		for (int i = 0; i < int(v.size()); ++i)
		{
			c.insert(std::make_pair(v[i], v[i]));
		}
		for (int i = 0; i < int(v.size()); ++i)
		{
			c.insert(std::make_pair(i, v[i]));
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
		val = 0;
		sw.Reset();
		for (int i = 0; i < int(v.size()); ++i)
		{
			auto ii = c.equal_range(v[i]);
			for (auto it = ii.first; it != ii.second; ++it)
			{
				val += it->second;
			}
		}
		for (int i = 0; i < int(v.size()); ++i)
		{
			auto ii = c.equal_range(i);
			for (auto it = ii.first; it != ii.second; ++it)
			{
				val += it->second;
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
		cout << "rbtree_multimap,std::greater" << endl;
		cout << "{" << endl;
		StopWatch sw;
		for (int i = 0; i < int(v.size()); ++i)
		{
			c.insert(std::make_pair(v[i], v[i]));
		}
		for (int i = 0; i < int(v.size()); ++i)
		{
			c.insert(std::make_pair(i, v[i]));
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
		val = 0;
		sw.Reset();
		for (int i = 0; i < int(v.size()); ++i)
		{
			auto ii = c.equal_range(v[i]);
			for (auto it = ii.first; it != ii.second; ++it)
			{
				val += it->second;
			}
		}
		for (int i = 0; i < int(v.size()); ++i)
		{
			auto ii = c.equal_range(i);
			for (auto it = ii.first; it != ii.second; ++it)
			{
				val += it->second;
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
	auto teststd_great = [&mtr, &mt, &c = stdmap_great, &v]()
	{
		cout << "std::multimap,std::greater" << endl;
		cout << "{" << endl;
		StopWatch sw;
		for (int i = 0; i < int(v.size()); ++i)
		{
			c.insert(std::make_pair(v[i], v[i]));
		}
		for (int i = 0; i < int(v.size()); ++i)
		{
			c.insert(std::make_pair(i, v[i]));
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
		val = 0;
		sw.Reset();
		for (int i = 0; i < int(v.size()); ++i)
		{
			auto ii = c.equal_range(v[i]);
			for (auto it = ii.first; it != ii.second; ++it)
			{
				val += it->second;
			}
		}
		for (int i = 0; i < int(v.size()); ++i)
		{
			auto ii = c.equal_range(i);
			for (auto it = ii.first; it != ii.second; ++it)
			{
				val += it->second;
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
		cout << "sbtree_multimap,std::greater" << endl;
		cout << "{" << endl;
		StopWatch sw;
		for (int i = 0; i < int(v.size()); ++i)
		{
			c.insert(std::make_pair(v[i], v[i]));
		}
		for (int i = 0; i < int(v.size()); ++i)
		{
			c.insert(std::make_pair(i, v[i]));
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
		val = 0;
		sw.Reset();
		for (int i = 0; i < int(v.size()); ++i)
		{
			auto ii = c.equal_range(v[i]);
			for (auto it = ii.first; it != ii.second; ++it)
			{
				val += it->second;
			}
		}
		for (int i = 0; i < int(v.size()); ++i)
		{
			auto ii = c.equal_range(i);
			for (auto it = ii.first; it != ii.second; ++it)
			{
				val += it->second;
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
	auto testavl_great = [&mtr, &mt, &c = avltree_great, &v]()
	{
		cout << "Ajy::avltree_multimap,std::greater" << endl;
		cout << "{" << endl;
		StopWatch sw;
		for (int i = 0; i < int(v.size()); ++i)
		{
			c.insert(std::make_pair(v[i], v[i]));
		}
		for (int i = 0; i < int(v.size()); ++i)
		{
			c.insert(std::make_pair(i, v[i]));
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
		val = 0;
		sw.Reset();
		for (int i = 0; i < int(v.size()); ++i)
		{
			auto ii = c.equal_range(v[i]);
			for (auto it = ii.first; it != ii.second; ++it)
			{
				val += it->second;
			}
		}
		for (int i = 0; i < int(v.size()); ++i)
		{
			auto ii = c.equal_range(i);
			for (auto it = ii.first; it != ii.second; ++it)
			{
				val += it->second;
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
	teststd_less();
	testredblack_less();
	testavl_less();
	testsb_less();
	reset();
	testbp_less();
	teststd_less();
	testredblack_less();
	testavl_less();
	testsb_less();
	reset();
	testbp_less();
	teststd_less();
	testredblack_less();
	testavl_less();
	testsb_less();

	cout << "=====================================" << endl;

	reset();
	testbp_great();
	teststd_great();
	testredblack_great();
	testavl_great();
	testsb_great();
	reset();
	testbp_great();
	teststd_great();
	testredblack_great();
	testavl_great();
	testsb_great();
	reset();
	testbp_great();
	teststd_great();
	testredblack_great();
	testavl_great();
	testsb_great();

	v.clear();

}
