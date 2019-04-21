#pragma once

void List_Test()
{
	List<int> lt;
	lt.PushBack(3);
	lt.PushBack(1);
	lt.PushBack(5);
	lt.PushFront(3);
	lt.PushBack(4);
	lt.PushBack(2);
	lt.RemoveIf([](int x) {
		return x == 3;
	});
	for (auto it = lt.Begin(); it != lt.End(); ++it)
	{
		std::cout << *it << std::endl;
	}
	cout << lt.Size() << endl;
	cout << "-------------------------" << endl;
	lt.Sort(false);
	for (auto it = lt.Begin(); it != lt.End(); ++it)
	{
		std::cout << *it << std::endl;
	}
	cout << lt.Size() << endl;
	cout << "-------------------------" << endl;
	lt.Sort();
	for (auto it = lt.Begin(); it != lt.End(); ++it)
	{
		std::cout << *it << std::endl;
	}
	cout << lt.Size() << endl;
	cout << "-------------------------" << endl;
	lt.Remove(3);
	for (auto it = lt.Begin(); it != lt.End(); ++it)
	{
		std::cout << *it << std::endl;
	}
	cout << lt.Size() << endl;
	cout << "-------------------------" << endl;
	lt.Reverse();
	for (auto it = lt.Begin(); it != lt.End(); ++it)
	{
		std::cout << *it << std::endl;
	}
	cout << lt.Size() << endl;
	cout << "-------------------------" << endl;
	for (auto it = --lt.End(); it != --lt.Begin(); --it)
	{
		std::cout << *it << std::endl;
	}
	cout << lt.Size() << endl;
	cout << "-------------------------" << endl;

	cout << "----------------------------------------------------------------" << endl;

	List<int> lt1;
	lt1.PushBack(100);
	lt1.Insert(lt1.End(), lt.Begin(), --lt.End());
	for (auto it = lt1.Begin(); it != lt1.End(); ++it)
	{
		std::cout << *it << std::endl;
	}
	cout << lt1.Size() << endl;
	cout << "-------------------------" << endl;
	lt1.Erase(++lt1.Begin(), --lt1.End());
	for (auto it = lt1.Begin(); it != lt1.End(); ++it)
	{
		std::cout << *it << std::endl;
	}
	cout << lt1.Size() << endl;
	cout << "-------------------------" << endl;
	lt1.Erase(lt1.Begin());
	for (auto it = lt1.Begin(); it != lt1.End(); ++it)
	{
		std::cout << *it << std::endl;
	}
	cout << lt1.Size() << endl;
	cout << "-------------------------" << endl;

	cout << "----------------------------------------------------------------" << endl;

	List<int> lt2(std::move(lt));
	for (auto it = lt2.Begin(); it != lt2.End(); ++it)
	{
		std::cout << *it << std::endl;
	}
	cout << "-------------------------" << endl;
	for (auto it = --lt2.End(); it != --lt2.Begin(); --it)
	{
		std::cout << *it << std::endl;
	}
	cout << "-------------------------" << endl;
	for (auto it = lt2.RBegin(); it != lt2.REnd(); ++it)
	{
		std::cout << *it << std::endl;
	}
	cout << "-------------------------" << endl;
	for (auto it = --lt2.REnd(); it != --lt2.RBegin(); --it)
	{
		std::cout << *it << std::endl;
	}
	
	cout << "----------------------------------------------------------------" << endl;

	lt.PushBack(1);
	lt.PushBack(2);
	lt.PushBack(3);
	lt.PushBack(4);
	lt.PushBack(5);
	for (auto it = lt.Begin(); it != lt.End(); ++it)
	{
		std::cout << *it << std::endl;
	}
	cout << "-------------------------" << endl;

	cout << "----------------------------------------------------------------" << endl;
}
