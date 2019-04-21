#pragma once

void Set_Test()
{
	Set<std::string> st;
	st.Insert(std::string("14"));
	st.Insert(std::string("9"));
	st.Insert(std::string("41"));
	st.Insert(std::string("39"));
	st.Insert(std::string("47"));
	st.Insert(std::string("20"));
	st.Insert(std::string("15"));
	st.Insert(std::string("22"));
	st.Insert(std::string("7"));
	st.Insert(std::string("3"));
	std::string s("28");
	st.Insert(std::move(s));
	s.append("24");
	st.Insert(s);
	cout << "isEmpty:   " << std::boolalpha << st.Empty() << endl;
	cout << "size:      " << st.Size() << endl;
	for (Set<std::string>::Iterator it = st.Begin(); it != st.End(); ++it)
	{
		cout << *it << endl;
	}
	cout << "----------------------------" << endl;
	for (Set<std::string>::Iterator it = --st.End(); it != --st.Begin(); --it)
	{
		cout << *it << endl;
	}
	cout << "----------------------------" << endl;
	for (Set<std::string>::ReverseIterator it = st.RBegin(); it != st.REnd(); ++it)
	{
		cout << *it << endl;
	}
	cout << "----------------------------" << endl;
	for (Set<std::string>::ReverseIterator it = --st.REnd(); it != --st.RBegin(); --it)
	{
		cout << *it << endl;
	}

	cout << "----------------------------------------------------------------" << endl;

	Set<std::string> st1;
	st1.Insert(st.Begin(), st.End());
	for (Set<std::string>::Iterator it = st1.Begin(); it != st1.End(); ++it)
	{
		cout << *it << endl;
	}
	cout << "----------------------------" << endl;

	cout << "----------------------------------------------------------------" << endl;

	Set<int> st2;
	List<int> lt;
	lt.PushBack(14);
	lt.PushBack(9);
	lt.PushBack(41);
	lt.PushBack(39);
	lt.PushBack(47);
	lt.PushBack(20);
	lt.PushBack(15);
	lt.PushBack(22);
	lt.PushBack(7);
	lt.PushBack(3);
	lt.PushBack(28);
	lt.PushBack(24);
	st2.Insert(lt.Begin(), lt.End());
	for (auto it = st2.Begin(); it != st2.End(); ++it)
	{
		cout << *it << endl;
	}
	cout << "----------------------------" << endl;
	
	cout << "----------------------------------------------------------------" << endl;

}
