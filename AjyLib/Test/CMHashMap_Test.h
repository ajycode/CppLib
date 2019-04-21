#pragma once

void CMHashMap_Test()
{
	/*CMHashMap<String, int> hmap;
	for (int i = 0; i < 10; ++i)
	{
		hmap.Add(String::MakeString(i), i);
	}
	for (int i = 0; i < hmap.count; ++i)
	{
		if (hmap.items[i].prev == -2) continue;
		std::cout << hmap.IndexAtKey(i).Std_str() << " " << hmap.IndexAtValue(i) << endl;;
	}

	CMHashMap<String, int> hmap1(hmap);
	hmap1.Remove(String::MakeString(1));
	for (int i = 0; i < hmap1.count; ++i)
	{
		if (hmap1.items[i].prev == -2) continue;
		std::cout << hmap1.IndexAtKey(i).Std_str() << " " << hmap1.IndexAtValue(i) << endl;;
	}*/

	CMHashMap<int, int> hmap;
	for (int i = 0; i < 10; ++i)
	{
		hmap.Add(i, i);
	}
	hmap.Remove(5);
	for (int i = 0; i < hmap.count; ++i)
	{
		if (hmap.items[i].prev == -2) continue;
		std::cout << hmap.IndexAtKey(i) << " " << hmap.IndexAtValue(i) << endl;;
	}

	CMHashMap<int, int> hmap1(hmap);
	hmap1.Remove(1);
	hmap1.Add(5, 5);
	for (int i = 0; i < hmap1.count; ++i)
	{
		if (hmap1.items[i].prev == -2) continue;
		std::cout << hmap1.IndexAtKey(i) << " " << hmap1.IndexAtValue(i) << endl;;
	}
	int idx = hmap1.Find(5);
	Cout.WriteLine(idx);
}
