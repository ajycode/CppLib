#pragma once

void BppTree_Test()
{

	bpptree_map<int, int> m;
	for (int i = 0; i < 20; ++i)
	{
		m.emplace(i, i);
	}
	cout << "size:   " << m.size() << endl;
	for (int i = 0; i < 20; ++i)
	{
		auto it = m.find(i);
		Cout.Write(it->first, " ",it->second, "     ");
	}
	Cout.WriteLine();
	for (auto it = m.begin(); it != m.end(); ++it)
	{
		Cout.Write(it->first, " ",it->second, "     ");
	}
	Cout.WriteLine();
	/*for (int i = 0; i < 20; ++i)
	{
		m.erase(i);
	}*/
	for (auto it = m.begin(); it != m.end();)
	{
		it = m.erase(it);
	}
	cout << "size:   " << m.size() << endl;
}

