#pragma once

void HashSet_Test()
{
	HashSet<std::string> hs;
	std::string s("helloworld");
	hs.Add(std::move(s));
	hs.Add("dsa");
	hs.Add("fgfh");
	hs.Add("isjlk");
	hs.Add("weisjd");
	hs.Add("djadj");
	hs.Add("opoijkl");
	auto data = hs.Data();
	for (int i = 0; i < data.DataLen(); ++i)
	{
		cout << "hashCode: " << data[i]->hash << "   key: " << data[i]->key << endl;
	}
	cout << "----------------------------------------" << endl;

	cout << "--------------------------------------------------------------------------------" << endl;

	HashSet<std::string> hs1(std::move(hs));
	//HashSet<std::string> hs1(hs);
	auto data1 = hs.Data();
	for (int i = 0; i < data1.DataLen(); ++i)
	{
		cout << "hashCode: " << data1[i]->hash << "   key: " << data1[i]->key << endl;
	}
	cout << "----------------------------------------" << endl;
	auto data2 = hs1.Data();
	for (int i = 0; i < data2.DataLen(); ++i)
	{
		cout << "hashCode: " << data2[i]->hash << "   key: " << data2[i]->key << endl;
	}
	cout << "----------------------------------------" << endl;

	cout << "--------------------------------------------------------------------------------" << endl;
}
