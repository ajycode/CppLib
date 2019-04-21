#pragma once

void CMHashSet_Test()
{

	CMHashSet<int> hmap;
	for (int i = 0; i < 10; ++i)
	{
		hmap.Add(i);
	}
	for (int i = 0; i < hmap.count; ++i)
	{
		if (hmap.items[i].prev == -2) continue;
		std::cout << hmap.IndexAtKey(i) << endl;;
	}

	CMHashSet<int> hmap1(hmap);
	hmap1.Remove(1);
	for (int i = 0; i < hmap1.count; ++i)
	{
		if (hmap1.items[i].prev == -2) continue;
		std::cout << hmap1.IndexAtKey(i) << endl;;
	}

	CMHashSet<String> hmap2;
	for (int i = 0; i < 10; ++i)
	{
		hmap2.Add(String::MakeString(i,i));
	}
	for (int i = 0; i < hmap2.count; ++i)
	{
		if (hmap2.items[i].prev == -2) continue;
		std::cout << hmap2.IndexAtKey(i).Std_str() << endl;;
	}

	CMHashSet<String> hmap3(hmap2);
	for (int i = 0; i < hmap3.count; ++i)
	{
		if (hmap3.items[i].prev == -2) continue;
		std::cout << hmap3.IndexAtKey(i).Std_str() << endl;;
	}

	CMHashSet<int> hset4(std::move(hmap));
	for (int i = 0; i < hset4.count; ++i)
	{
		if (hset4.items[i].prev == -2) continue;
		std::cout << hset4.IndexAtKey(i) << endl;;
	}
}
