#pragma once

struct RedBlackTree_Test_Helper
{
	int year;
	double country;
	std::string name;

	RedBlackTree_Test_Helper()
		: year(-1)
		, country(0.123456789)
		, name("empty")
	{
	}

	RedBlackTree_Test_Helper(int a, double b, string c)
		: year(a)
		, country(b)
		, name(c)
	{
		cout << "is constucted" << endl;
	}

	RedBlackTree_Test_Helper(RedBlackTree_Test_Helper const& other)
		: year(other.year)
		, country(other.country)
		, name(std::move(other.name))
	{
		cout << "is moved" << endl;
	}
};

void RedBlackTree_Test()
{
	/************************************************************************/
	/* map_test                                                             */
	/************************************************************************/

	/*rbtree_map<int, int> mp, mp1;
	mp.emplace_hint(mp.cend(), -2, -2);
	mp.emplace(-1, -1);
	for (int i = 0; i < 15; ++i)
	{
		mp.insert(std::make_pair(i, i));
	}
	mp.emplace_hint(--mp.cend(), -3, -3);
	cout << "mp.size:   " << mp.size() << endl;
	for (int i = -3; i < 15; ++i)
	{
		auto it = mp.equal_range(i);
		for (auto o = it.first; o != it.second; ++o)
		{
			cout << o->first << "   " << o->second << endl;
		}
	}
	mp1 = mp;
	cout << "mp1.size:   " << mp1.size() << endl;
	for (auto it = mp1.begin(); it != mp1.end(); ++it)
	{
		cout << it->first << "   " << it->second << endl;
	}
	for (int i = 0; i < 15; ++i)
	{
		auto it = mp.find(i);
		cout << it->first << "   " << it->second << endl;
	}
	for (int i = 0; i < 15; ++i)
	{
		cout << mp.at(i) << endl;
	}
	mp1[15] = 15;
	for (int i = 0; i < 16; ++i)
	{
		cout << mp1[i] << endl;
	}
	rbtree_map<int, int> mp2(std::move(mp1));
	cout << "mp.size:   " << mp.size() << endl;
	cout << "mp1.size:   " << mp1.size() << endl;
	cout << "mp2.size:   " << mp2.size() << endl;
	for (auto it = mp2.begin(); it != mp2.end(); ++it)
	{
		cout << it->first << "   " << it->second << endl;
	}
	mp1[0] = 0;
	mp1[1] = 1;
	mp1[-1] = -1;
	mp1[2] = 2;
	mp1[-2] = -2;
	mp1.at(0);
	cout << "mp1.size:   " << mp1.size() << endl;
	for (auto it = mp1.begin(); it != mp1.end(); ++it)
	{
		cout << it->first << "   " << it->second << endl;
	}
	for (auto it = mp2.begin(); it != mp2.end();)
	{
		it = mp2.erase(it);
	}
	cout << "mp2.size:   " << mp2.size() << endl;
	for (int i = -2; i < 3; ++i)
	{
		mp1.erase(i);
	}
	cout << "mp1.size:   " << mp1.size() << endl;
	cout << "========================================================" << endl;*/

	/************************************************************************/
	/* multi_map															*/
	/************************************************************************/

	rbtree_multimap<int, int> multimp, multimp1;
	multimp.emplace_hint(multimp.cend(), -2, -2);
	multimp.emplace_hint(multimp.cbegin(), -2, -2);
	multimp.emplace_hint(multimp.cbegin(), -2, -2);
	multimp.emplace(-1, -1);
	multimp.emplace(-1, -1);
	multimp.emplace(-1, -1);
	multimp.emplace(5, 5);
	multimp.emplace(6, 6);
	multimp.emplace(7, 7);
	for (int i = 0; i < 5; ++i)
	{
		multimp.insert(std::make_pair(i, i));
	}
	for (int i = 0; i < 5; ++i)
	{
		multimp.insert(std::make_pair(i, i));
	}
	for (int i = 0; i < 5; ++i)
	{
		multimp.insert(std::make_pair(i, i));
	}
	cout << multimp.count(-2) << endl;
	cout << "multimp.size:   " << multimp.size() << endl;
	for (auto it = multimp.begin(); it != multimp.end(); ++it)
	{
		cout << it->first << "   " << it->second << endl;
	}
	for (int i = 0; i < 5; ++i)
	{
		auto it = multimp.equal_range(i);
		for (auto o = it.first; o != it.second; ++o)
		{
			cout << o->first << "   " << o->second << endl;
		}
	}
	for (int i = -1; i < 5; ++i)
	{
		multimp.erase(i);
	}
	cout << "multimp.size:   " << multimp.size() << endl;
	for (auto it = multimp.begin(); it != multimp.end(); ++it)
	{
		cout << it->first << "   " << it->second << endl;
	}
	cout << "=======================" << endl;
	for (auto it = --multimp.end(); it != --multimp.begin(); --it)
	{
		cout << it->first << "   " << it->second << endl;
	}
	cout << "=======================" << endl;
	for (auto it = multimp.rbegin(); it != multimp.rend(); ++it)
	{
		cout << it->first << "   " << it->second << endl;
	}
	cout << "=======================" << endl;
	for (auto it = --multimp.rend(); it != --multimp.rbegin(); --it)
	{
		cout << it->first << "   " << it->second << endl;
	}
	multimp1 = std::move(multimp);
	cout << "multimp.size:   " << multimp.size() << endl;
	cout << "multimp1.size:   " << multimp1.size() << endl;
	for (auto it = multimp1.begin(); it != multimp1.end();)
	{
		it = multimp1.erase(it);
	}
	cout << "multimp1.size:   " << multimp1.size() << endl;
	cout << "========================================================" << endl;

	/************************************************************************/
	/* set_test																*/
	/************************************************************************/

	/*rbtree_set<int, std::greater<int>> st;
	st.emplace_hint(st.cend(), -2);
	st.emplace(-1);
	for (int i = 0; i < 15; ++i)
	{
		st.emplace(i);
	}
	cout <<"st.size:   "<< st.size() << endl;
	rbtree_set<int, std::greater<int>> st1(st);
	cout << "st1.size:   " << st1.size() << endl;
	for (auto it = st1.begin(); it != st1.end(); ++it)
	{
		cout << *it << endl;
	}
	rbtree_set<int, std::greater<int>> st2;
	st2.insert(std::initializer_list<int>{0, 1, 2, 3, 4, 4, 5, 6, 6, 7, 8, 9});
	cout << "st2.size:   " << st2.size() << endl;
	for (auto it = st2.begin(); it != st2.end(); ++it)
	{
		cout << *it << endl;
	}
	rbtree_set<int, std::greater<int>> st3;
	st3.insert(st2.begin(), st2.end());
	cout << "st3.size:   " << st3.size() << endl;
	for (auto it = st3.begin(); it != st3.end(); ++it)
	{
		cout << *it << endl;
	}
	cout << "========================================================" << endl;*/

	/************************************************************************/
	/* multiset_test														*/
	/************************************************************************/

	/*rbtree_multiset<int,std::greater<int>> multist;
	multist.emplace_hint(multist.cbegin(), -2);
	multist.emplace_hint(multist.cend(), -2);
	multist.emplace_hint(multist.cend(), -2);
	multist.emplace(-1);
	multist.emplace(-1);
	multist.emplace(-1);
	for (int i = 0; i < 5;++i)
	{
		multist.insert(i);
	}
	for (int i = 0; i < 5; ++i)
	{
		multist.insert(i);
	}
	for (int i = 0; i < 5; ++i)
	{
		multist.insert(i);
	}
	cout <<"multist.size:   "<< multist.size() << endl;
	for (auto it = multist.begin(); it != multist.end(); ++it)
	{
		cout << *it << endl;
	}
	for (int i = 0; i < 5; ++i)
	{
		auto it = multist.equal_range(i);
		for (auto o = it.first; o != it.second; ++o)
		{
			cout << *o << endl;
		}
	}
	for (int i = -2; i < 5; ++i)
	{
		multist.erase(i);
	}
	cout << "multist.size:   " << multist.size() << endl;
	for (auto it = multist.begin(); it != multist.end(); ++it)
	{
		cout << *it << endl;
	}
	rbtree_multiset<int, std::greater<int>> multist1(multist.begin(),multist.end());
	cout << "multist1.size:   " << multist1.size() << endl;
	for (auto it = multist1.begin(); it != multist1.end(); ++it)
	{
		cout << *it << endl;
	}
	rbtree_multiset<int, std::greater<int>> multist2(std::initializer_list<int>{0, 1, 2, 3, 4, 4, 5, 6, 6, 7, 8, 9});
	cout << "multist2.size:   " << multist2.size() << endl;
	for (auto it = multist2.begin(); it != multist2.end(); ++it)
	{
		cout << *it << endl;
	}
	cout << "========================================================" << endl;*/

	/************************************************************************/
	/* other_test															*/
	/************************************************************************/

	/*std::map<int, RedBlackTree_Test_Helper> mp;
	rbtree_map<int, RedBlackTree_Test_Helper> mp1;
	int anInt = 4;
	double aDouble = 5.0;
	std::string aString = "C++";
	cout << "！insert--" << endl;
	mp.insert(std::make_pair(4, RedBlackTree_Test_Helper(anInt, aDouble, aString)));

	cout << "！emplace--" << endl;
	mp.emplace(4, RedBlackTree_Test_Helper(anInt, aDouble, aString));
	cout << "========================================================" << endl;

	cout << "！insert--" << endl;
	mp1.insert(std::make_pair(4, RedBlackTree_Test_Helper(anInt, aDouble, aString)));

	cout << "！emplace--" << endl;
	mp1.emplace(4, RedBlackTree_Test_Helper(anInt, aDouble, aString));
	cout << "========================================================" << endl;*/

}

