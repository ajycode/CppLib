#pragma once

void Dictionary_Test()
{
	Dictionary<int, int> dict;
	dict.Add(1, 1);
	dict.Add(2, 2);
	dict.Add(3, 3);
	dict.Add(4, 4);
	dict.Add(5, 5);
	for (int i = 1; i < 6; ++i)
	{
		auto o = dict[i];
		cout << o << endl;
	}

	Dictionary<int, int> dict1(dict);
	for (int i = 1; i < 6; ++i)
	{
		auto o = dict1[i];
		cout << o << endl;
	}
}
