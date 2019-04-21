#pragma once

void MultiSet_Test()
{
	MultiSet<int> mulst;
	mulst.Insert(14);
	mulst.Insert(9);
	mulst.Insert(41);
	mulst.Insert(39);
	mulst.Insert(47);
	mulst.Insert(20);
	mulst.Insert(15);
	mulst.Insert(22);
	mulst.Insert(7);
	mulst.Insert(3);
	mulst.Insert(28);
	mulst.Insert(24);
	mulst.Insert(14);
	mulst.Insert(22);
	mulst.Insert(2);
	mulst.Insert(47);
	cout << "isEmpty:   " << std::boolalpha << mulst.Empty() << endl;
	cout << "size:      " << mulst.Size() << endl;
	for (MultiSet<int>::Iterator it = mulst.Begin(); it != mulst.End(); ++it)
	{
		cout << *it << endl;
	}
	cout << "----------------------------" << endl;
	for (MultiSet<int>::Iterator it = --mulst.End(); it != --mulst.Begin(); --it)
	{
		cout << *it << endl;
	}
	cout << "----------------------------" << endl;
	for (MultiSet<int>::ReverseIterator it = mulst.RBegin(); it != mulst.REnd(); ++it)
	{
		cout << *it << endl;
	}
	cout << "----------------------------" << endl;
	for (MultiSet<int>::ReverseIterator it = --mulst.REnd(); it != --mulst.RBegin(); --it)
	{
		cout << *it << endl;
	}
	cout << "----------------------------" << endl;

	cout << "----------------------------------------------------------------" << endl;
}
