#pragma once

void Stack_Test()
{
	Stack<List<String>> stk;
	List<String> lt;
	String s("a");
	String s1("b");
	String s2("c");
	String s3("d");
	String s4("e");
	String s5("f");
	String s6("g");
	String s7("h");
	String s8("i");
	String s9("j");
	lt.PushBack(std::move(s));
	lt.PushBack((String&&)s1);
	lt.PushBack(s2);
	lt.PushBack(s3);
	lt.PushBack(s4);
	lt.PushBack(s5);
	lt.PushBack(s6);
	lt.PushBack(s7);
	lt.PushBack(s8);
	lt.PushBack(s9);
	stk.Push(std::move(lt));
	//vec.Push(lt);
	auto& o = stk.Top();
	for (auto it = o.Begin(); it != o.End(); ++it)
	{
		cout << (*it).Std_str() << endl;
	}

	cout << "-------------------------------------------------------------" << endl;

	Stack<String> stk1;
	s.Append("a");
	s1.Push('b');
	stk1.Push(s);
	stk1.Push(s1);
	stk1.Push(s2);
	stk1.Push(s3);
	stk1.Push(s4);
	stk1.Push(s5);
	stk1.Push(s6);
	stk1.Push(s7);
	stk1.Push(s8);
	stk1.Push(s9);
	while (!stk1.IsEmpty())
	{
		auto& o = stk1.Top();
		cout << o.Std_str() << endl;
		stk1.Pop();
	}

	cout << "-------------------------------------------------------------" << endl;

	Stack<std::string> stk2;
	stk2.Push(std::string("helloworld"));
	stk2.Push(std::string("helloworl"));
	stk2.Push(std::string("hellowor"));
	stk2.Push(std::string("hellowo"));
	stk2.Push(std::string("hellow"));
	stk2.Push(std::string("hello"));
	stk2.Push(std::string("hell"));
	stk2.Push(std::string("hel"));
	stk2.Push(std::string("he"));
	stk2.Push(std::string("h"));
	while (!stk2.IsEmpty())
	{
		auto& o = stk2.Top();
		cout << o << endl;
		stk2.Pop();
	}

	cout << "-------------------------------------------------------------" << endl;
}
