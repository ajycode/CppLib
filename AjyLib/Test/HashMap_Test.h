#pragma once

void HashMap_Test()
{
	HashMap<double, std::string> hs;
	std::string s("helloworld");
	hs.Add(1.111, std::move(s));
	hs.Add(2.222, "dsa");
	hs.Add(3.333, "fgfh");
	hs.Add(4.444, "isjlk");
	hs.Add(5.555, "weisjd");
	hs.Add(6.666, "djadj");
	hs.Add(7.777, "opoijkl");
	auto data = hs.Data();
	for (int i = 0; i < data.DataLen(); ++i)
	{
		cout << "hashCode: " << data[i]->hash << "   key: " << data[i]->key << "   val: " << data[i]->value << endl;
	}
	cout << "----------------------------------------" << endl;

	cout << "--------------------------------------------------------------------------------" << endl;

	HashMap<double, std::string> hs1(std::move(hs));
	//HashMap<double, std::string> hs1(hs);
	auto data1 = hs.Data();
	for (int i = 0; i < data1.DataLen(); ++i)
	{
		cout << "hashCode: " << data1[i]->hash << "   key: " << data1[i]->key << "   val: " << data1[i]->value << endl;
	}
	cout << "----------------------------------------" << endl;
	auto data2 = hs1.Data();
	for (int i = 0; i < data2.DataLen(); ++i)
	{
		cout << "hashCode: " << data2[i]->hash << "   key: " << data2[i]->key << "   val: " << data2[i]->value << endl;
	}
	cout << "----------------------------------------" << endl;

	cout << "--------------------------------------------------------------------------------" << endl;

	Vector<double> vec;
	vec.Push(1.111);
	vec.Push(2.222);
	vec.Push(3.333);
	vec.Push(4.444);
	vec.Push(5.555);
	vec.Push(6.666);
	vec.Push(7.777);
	for (int i = 0; i < vec.DataLen();++i)
	{
		auto o = hs1.Find(vec[i]);
		cout << "key: " << o->key << "   val: " << o->value << endl;
	}
	cout << "----------------------------------------" << endl;

	cout << "--------------------------------------------------------------------------------" << endl;
}
