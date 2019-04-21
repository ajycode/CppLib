#pragma once

void Queue_Test()
{
	Queue<List<String>> que;
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
	que.Push(std::move(lt));
	//vec.Push(lt);
	auto& o = que.Front();
	for (auto it = o.Begin(); it != o.End(); ++it)
	{
		cout << (*it).Std_str() << endl;
	}

	cout << "-------------------------------------------------------------" << endl;

	Queue<String> que1;
	s.Append("a");
	s1.Push('b');
	que1.Push(s);
	que1.Push(s1);
	que1.Push(s2);
	que1.Push(s3);
	que1.Push(s4);
	que1.Push(s5);
	que1.Push(s6);
	que1.Push(s7);
	que1.Push(s8);
	que1.Push(s9);
	while (!que1.Empty())
	{
		auto& o = que1.Front();
		cout << o.Std_str() << endl;
		que1.Pop();
	}

	cout << "-------------------------------------------------------------" << endl;

	Queue<std::string> que2;
	que2.Push(std::string("helloworld"));
	que2.Push(std::string("helloworl"));
	que2.Push(std::string("hellowor"));
	que2.Push(std::string("hellowo"));
	que2.Push(std::string("hellow"));
	que2.Push(std::string("hello"));
	que2.Push(std::string("hell"));
	que2.Push(std::string("hel"));
	que2.Push(std::string("he"));
	que2.Push(std::string("h"));
	while (!que2.Empty())
	{
		auto& o = que2.Front();
		cout << o << endl;
		que2.Pop();
	}

	cout << "-------------------------------------------------------------" << endl;
}
