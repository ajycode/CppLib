#pragma once

void Map_Test()
{
	//Map<int, int> mp;
	//mp.Insert(std::pair<int, int>(14, 14));
	//mp.Insert(std::pair<int, int>(9, 9));
	//mp.Insert(std::pair<int, int>(41, 41));
	//mp.Insert(std::pair<int, int>(39, 39));
	//mp.Insert(std::pair<int, int>(47, 47));
	//mp.Insert(std::pair<int, int>(20, 20));
	//mp.Insert(std::pair<int, int>(15, 15));
	//mp.Insert(std::pair<int, int>(22, 22));
	//mp.Insert(std::pair<int, int>(7, 7));
	//mp.Insert(std::pair<int, int>(3, 3));
	//mp.Insert(std::pair<int, int>(28, 28));
	//mp.Insert(std::pair<int, int>(24, 24));
	//mp.Insert(std::pair<int, int>(20, 200));
	//mp.Insert(std::pair<int, int>(20, 200), true);
	////mp.Delete(++mp.Begin(), --mp.End());
	///*mp.Delete(22);
	//mp.Delete(24);
	//mp.Delete(39);
	//mp.Delete(28);
	//mp.Delete(20);
	//mp.Delete(14);
	//mp.Delete(3);
	//mp.Delete(47);
	//mp.Delete(41);
	//mp.Delete(9);
	//mp.Delete(15);
	//mp.Delete(7);*/
	////mp.Clear();
	///*auto it = mp.Find(20);
	//auto it_2 = mp.Find(28);
	//mp.Delete(it, it_2);*/
	//cout << "isEmpty:   " << std::boolalpha << mp.Empty() << endl;
	//cout << "size:      " << mp.Size() << endl;
	//for (Map<int, int>::Iterator it = mp.Begin(); it != mp.End(); ++it)
	//{
	//	cout << it->first << "   " << it->second << endl;
	//}
	//cout << "----------------------------" << endl;
	//for (Map<int, int>::Iterator it = --mp.End(); it != --mp.Begin(); --it)
	//{
	//	cout << it->first << "   " << it->second << endl;
	//}
	//cout << "----------------------------" << endl;
	//for (Map<int, int>::ReverseIterator it = mp.RBegin(); it != mp.REnd(); ++it)
	//{
	//	cout << it->first << "   " << it->second << endl;
	//}
	//cout << "----------------------------" << endl;
	//for (Map<int, int>::ReverseIterator it = --mp.REnd(); it != --mp.RBegin(); --it)
	//{
	//	cout << it->first << "   " << it->second << endl;
	//}
	//cout << "----------------------------" << endl;

	//mp[20] = 20;
	//mp[100];
	//cout << "isEmpty:   " << std::boolalpha << mp.Empty() << endl;
	//cout << "size:      " << mp.Size() << endl;
	//for (Map<int, int>::Iterator it = mp.Begin(); it != mp.End(); ++it)
	//{
	//	cout << it->first << "   " << it->second << endl;
	//}
	//cout << "----------------------------" << endl;
	//for (Map<int, int>::Iterator it = --mp.End(); it != --mp.Begin(); --it)
	//{
	//	cout << it->first << "   " << it->second << endl;
	//}
	//cout << "----------------------------" << endl;
	//for (Map<int, int>::ReverseIterator it = mp.RBegin(); it != mp.REnd(); ++it)
	//{
	//	cout << it->first << "   " << it->second << endl;
	//}
	//cout << "----------------------------" << endl;
	//for (Map<int, int>::ReverseIterator it = --mp.REnd(); it != --mp.RBegin(); --it)
	//{
	//	cout << it->first << "   " << it->second << endl;
	//}
	//cout << "----------------------------" << endl;

	//cout << "----------------------------------------------------------------" << endl;

	//Map<int, int> mp1(mp);
	////mp.Delete(++mp.Begin(), --mp.End());
	///*mp.Delete(22);
	//mp.Delete(24);
	//mp.Delete(39);
	//mp.Delete(28);
	//mp.Delete(20);
	//mp.Delete(14);
	//mp.Delete(3);
	//mp.Delete(47);
	//mp.Delete(41);
	//mp.Delete(9);
	//mp.Delete(15);
	//mp.Delete(7);*/
	////mp.Clear();
	//auto it1 = mp1.Find(7);
	//mp1.Delete(it1, --mp1.End());
	//cout << "isEmpty:   " << std::boolalpha << mp1.Empty() << endl;
	//cout << "size:      " << mp1.Size() << endl;
	//for (Map<int, int>::Iterator it = mp1.Begin(); it != mp1.End(); ++it)
	//{
	//	cout << it->first << "   " << it->second << endl;
	//}
	//cout << "----------------------------" << endl;
	//for (Map<int, int>::Iterator it = --mp1.End(); it != --mp1.Begin(); --it)
	//{
	//	cout << it->first << "   " << it->second << endl;
	//}
	//cout << "----------------------------" << endl;
	//for (Map<int, int>::ReverseIterator it = mp1.RBegin(); it != mp1.REnd(); ++it)
	//{
	//	cout << it->first << "   " << it->second << endl;
	//}
	//cout << "----------------------------" << endl;
	//for (Map<int, int>::ReverseIterator it = --mp1.REnd(); it != --mp1.RBegin(); --it)
	//{
	//	cout << it->first << "   " << it->second << endl;
	//}
	//cout << "----------------------------" << endl;

	//cout << "----------------------------------------------------------------" << endl;

	//mp1 = mp;
	//cout << "isEmpty:   " << std::boolalpha << mp1.Empty() << endl;
	//cout << "size:      " << mp1.Size() << endl;
	//for (Map<int, int>::Iterator it = mp1.Begin(); it != mp1.End(); ++it)
	//{
	//	cout << it->first << "   " << it->second << endl;
	//}
	//cout << "----------------------------" << endl;

	//cout << "----------------------------------------------------------------" << endl;

	Map<int, int> mp2;
	mp2.Insert(std::pair<int, int>(14, 14));
	mp2.Insert(std::pair<int, int>(9, 9));
	mp2.Insert(std::pair<int, int>(41, 41));
	mp2.Insert(std::pair<int, int>(39, 39));
	mp2.Insert(std::pair<int, int>(47, 47));
	mp2.Insert(std::pair<int, int>(20, 20));
	mp2.Insert(std::pair<int, int>(15, 15));
	mp2.Insert(std::pair<int, int>(22, 22));
	cout << "isEmpty:   " << std::boolalpha << mp2.Empty() << endl;
	cout << "size:      " << mp2.Size() << endl;
	for (Map<int, int>::Iterator it = mp2.Begin(); it != mp2.End(); ++it)
	{
		cout << it->first << "   " << it->second << endl;
	}
	cout << "----------------------------" << endl;
	for (Map<int, int>::Iterator it = --mp2.End(); it != --mp2.Begin(); --it)
	{
		cout << it->first << "   " << it->second << endl;
	}
	cout << "----------------------------" << endl;
	for (Map<int, int>::ReverseIterator it = mp2.RBegin(); it != mp2.REnd(); ++it)
	{
		cout << it->first << "   " << it->second << endl;
	}
	cout << "----------------------------" << endl;
	for (Map<int, int>::ReverseIterator it = --mp2.REnd(); it != --mp2.RBegin(); --it)
	{
		cout << it->first << "   " << it->second << endl;
	}
	cout << "----------------------------" << endl;

	cout << "--------------------------------1--------------------------------" << endl;

	Map<int, int> mp3(std::move(mp2));
	cout << "isEmpty:   " << std::boolalpha << mp3.Empty() << endl;
	cout << "size:      " << mp3.Size() << endl;
	for (Map<int, int>::Iterator it = mp3.Begin(); it != mp3.End(); ++it)
	{
		cout << it->first << "   " << it->second << endl;
	}
	cout << "----------------------------" << endl;
	for (Map<int, int>::Iterator it = --mp3.End(); it != --mp3.Begin(); --it)
	{
		cout << it->first << "   " << it->second << endl;
	}
	cout << "----------------------------" << endl;
	for (Map<int, int>::ReverseIterator it = mp3.RBegin(); it != mp3.REnd(); ++it)
	{
		cout << it->first << "   " << it->second << endl;
	}
	cout << "----------------------------" << endl;
	for (Map<int, int>::ReverseIterator it = --mp3.REnd(); it != --mp3.RBegin(); --it)
	{
		cout << it->first << "   " << it->second << endl;
	}
	cout << "----------------------------" << endl;

	cout << "-------------------------------2---------------------------------" << endl;

	mp3.Insert(std::pair<int, int>(7, 7));
	mp3.Insert(std::pair<int, int>(3, 3));
	mp3.Insert(std::pair<int, int>(28, 28));
	mp3.Insert(std::pair<int, int>(24, 24));
	cout << "isEmpty:   " << std::boolalpha << mp3.Empty() << endl;
	cout << "size:      " << mp3.Size() << endl;
	for (Map<int, int>::Iterator it = mp3.Begin(); it != mp3.End(); ++it)
	{
		cout << it->first << "   " << it->second << endl;
	}
	cout << "----------------------------" << endl;
	for (Map<int, int>::Iterator it = --mp3.End(); it != --mp3.Begin(); --it)
	{
		cout << it->first << "   " << it->second << endl;
	}
	cout << "----------------------------" << endl;
	for (Map<int, int>::ReverseIterator it = mp3.RBegin(); it != mp3.REnd(); ++it)
	{
		cout << it->first << "   " << it->second << endl;
	}
	cout << "----------------------------" << endl;
	for (Map<int, int>::ReverseIterator it = --mp3.REnd(); it != --mp3.RBegin(); --it)
	{
		cout << it->first << "   " << it->second << endl;
	}
	cout << "----------------------------" << endl;

	cout << "-----------------------------3-----------------------------------" << endl;

	mp2.Insert(std::pair<int, int>(14, 14));
	mp2.Insert(std::pair<int, int>(9, 9));
	mp2.Insert(std::pair<int, int>(41, 41));
	mp2.Insert(std::pair<int, int>(39, 39));
	mp2.Insert(std::pair<int, int>(47, 47));
	mp2.Insert(std::pair<int, int>(20, 20));
	mp2.Insert(std::pair<int, int>(15, 15));
	mp2.Insert(std::pair<int, int>(22, 22));
	mp2.Insert(std::pair<int, int>(7, 7));
	mp2.Insert(std::pair<int, int>(3, 3));
	mp2.Insert(std::pair<int, int>(28, 28));
	mp2.Insert(std::pair<int, int>(24, 24));
	cout << "isEmpty:   " << std::boolalpha << mp2.Empty() << endl;
	cout << "size:      " << mp2.Size() << endl;
	for (Map<int, int>::Iterator it = mp2.Begin(); it != mp2.End(); ++it)
	{
		cout << it->first << "   " << it->second << endl;
	}
	cout << "----------------------------" << endl;
	for (Map<int, int>::Iterator it = --mp2.End(); it != --mp2.Begin(); --it)
	{
		cout << it->first << "   " << it->second << endl;
	}
	cout << "----------------------------" << endl;
	for (Map<int, int>::ReverseIterator it = mp2.RBegin(); it != mp2.REnd(); ++it)
	{
		cout << it->first << "   " << it->second << endl;
	}
	cout << "----------------------------" << endl;
	for (Map<int, int>::ReverseIterator it = --mp2.REnd(); it != --mp2.RBegin(); --it)
	{
		cout << it->first << "   " << it->second << endl;
	}
	cout << "----------------------------" << endl;

	cout << "---------------------------------4-------------------------------" << endl;

	mp2.Clear();
	mp2 = std::move(mp3);
	cout << "isEmpty:   " << std::boolalpha << mp2.Empty() << endl;
	cout << "size:      " << mp2.Size() << endl;
	for (Map<int, int>::Iterator it = mp2.Begin(); it != mp2.End(); ++it)
	{
		cout << it->first << "   " << it->second << endl;
	}
	cout << "----------------------------" << endl;
	for (Map<int, int>::Iterator it = --mp2.End(); it != --mp2.Begin(); --it)
	{
		cout << it->first << "   " << it->second << endl;
	}
	cout << "----------------------------" << endl;
	for (Map<int, int>::ReverseIterator it = mp2.RBegin(); it != mp2.REnd(); ++it)
	{
		cout << it->first << "   " << it->second << endl;
	}
	cout << "----------------------------" << endl;
	for (Map<int, int>::ReverseIterator it = --mp2.REnd(); it != --mp2.RBegin(); --it)
	{
		cout << it->first << "   " << it->second << endl;
	}
	cout << "----------------------------" << endl;

	cout << "---------------------------------5-------------------------------" << endl;

	Map<int, std::string> mp4;
	std::pair<int, std::string> p(1, "helloworld");
	mp4.Insert(std::move(p));
	for (auto it = mp4.Begin(); it != mp4.End(); ++it)
	{
		cout << it->first << "   " << it->second << endl;
	}
	cout << "----------------------------" << endl;

	cout << "---------------------------------6-------------------------------" << endl;
}
