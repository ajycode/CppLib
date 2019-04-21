#pragma once

void MultiMap_test()
{
	MultiMap<int, std::string> mulmp;
	mulmp.Insert(std::pair<int, std::string>(14, "14"));
	mulmp.Insert(std::pair<int, std::string>(9, "9"));
	mulmp.Insert(std::pair<int, std::string>(41, "41"));
	mulmp.Insert(std::pair<int, std::string>(39, "39"));
	mulmp.Insert(std::pair<int, std::string>(47, "47"));
	mulmp.Insert(std::pair<int, std::string>(20, "20"));
	mulmp.Insert(std::pair<int, std::string>(15, "15"));
	mulmp.Insert(std::pair<int, std::string>(22, "22"));
	mulmp.Insert(std::pair<int, std::string>(7, "7"));
	mulmp.Insert(std::pair<int, std::string>(3, "3"));
	mulmp.Insert(std::pair<int, std::string>(28, "28"));
	mulmp.Insert(std::pair<int, std::string>(24, "24"));
	mulmp.Insert(std::pair<int, std::string>(14, "140"));
	mulmp.Insert(std::pair<int, std::string>(22, "220"));
	mulmp.Insert(std::pair<int, std::string>(2, "2"));
	std::pair<int, std::string> p(47, "470");
	mulmp.Insert(std::move(p));
	/*auto num = mulmp.Delete(22);
	auto num1 = mulmp.Delete(47);*/
	/*auto it = mulmp.Find(15);
	auto it2 = mulmp.Find(28);
	mulmp.Delete(++mulmp.Begin(), --mulmp.End());*/
	cout << "isEmpty:   " << std::boolalpha << mulmp.Empty() << endl;
	cout << "size:      " << mulmp.Size() << endl;
	for (MultiMap<int, std::string>::Iterator it = mulmp.Begin(); it != mulmp.End(); ++it)
	{
		cout << it->first << "   " << it->second << endl;
	}
	cout << "----------------------------" << endl;
	for (MultiMap<int, std::string>::Iterator it = --mulmp.End(); it != --mulmp.Begin(); --it)
	{
		cout << it->first << "   " << it->second << endl;
	}
	cout << "----------------------------" << endl;
	for (MultiMap<int, std::string>::ReverseIterator it = mulmp.RBegin(); it != mulmp.REnd(); ++it)
	{
		cout << it->first << "   " << it->second << endl;
	}
	cout << "----------------------------" << endl;
	for (MultiMap<int, std::string>::ReverseIterator it = --mulmp.REnd(); it != --mulmp.RBegin(); --it)
	{
		cout << it->first << "   " << it->second << endl;
	}
	cout << "----------------------------" << endl;

	mulmp.~MultiMap();
	cout << "----------------------------------------------------------------" << endl;
}
