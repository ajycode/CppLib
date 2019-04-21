#pragma once

void Serialization_Test_1()
{
	//String换成std::string结果一致
	HashMap<int, Vector<std::string>> hmp, hmp1;
	Vector<std::string> vec1, vec2, vec3;
	std::string s0("asdhf"), s1("dsafsad"), s2("dfgsdhf");
	std::string s3("java"), s4("ifdoisgn"), s5("212sdaf");
	std::string s6("-dsa6621"), s7("pldsfadf"), s8("cplusplus");
	vec1.Push(s0, s1, s2);
	vec2.Push(s3, s4, s5);
	vec3.Push(s6, s7, s8);
	hmp.Add(-1, vec1);
	hmp.Add(0, vec2);
	hmp.Add(1, vec3);
	ByteBuffer bb;
	bb.Write(hmp);
	for (size_t i = 0; i < bb.dataLen; ++i)
	{
		printf("%02X ", (unsigned char)bb.buf[i]);
	}
	cout << endl;
	bb.Read(hmp1);
	auto data = hmp1.Data();
	for (int i = 0; i < data.DataLen(); ++i)
	{
		auto o = data[i];
		cout << "Key: " << o->key << "   Value: ";
		for (int j = 0; j < o->value.DataLen(); ++j)
		{
			cout << o->value[j] << " ";
		}
		cout << endl;
	}
	cout << "===================================================" << endl;
}

void Serialization_Test_2()
{
	MultiSet<std::string> st, st1;
	st.Insert("-1000");
	st.Insert("2000");
	st.Insert("1000");
	st.Insert("1000");
	st.Insert("2000");
	st.Insert("-1000");
	ByteBuffer bb;
	bb.Write(st);
	for (size_t i = 0; i < bb.dataLen; ++i)
	{
	printf("%02X ", (unsigned char)bb.buf[i]);
	}
	cout << endl;
	bb.Read(st1);
	for (auto it = st1.Begin(); it != st1.End(); ++it)
	{
	cout << *it << endl;
	}
	cout << "===================================================" << endl;
}

void Serialization_Test_3()
{
	Map<std::string, List<String>> mp, mp1;
	List<String> lt1, lt2, lt3;
	String s0("asdhf"), s1("dsafsad"), s2("dfgsdhf");
	String s3("java"), s4("ifdoisgn"), s5("212sdaf");
	String s6("-dsa6621"), s7("pldsfadf"), s8("");
	s8.Append(-0.123456789, s3, false, "cplusplus", true, std::string("lua"));
	lt1.PushBack(s0);
	lt1.PushBack(s1);
	lt1.PushBack(s2);
	lt2.PushBack(s3);
	lt2.PushBack(s4);
	lt2.PushBack(s5);
	lt3.PushBack(s6);
	lt3.PushBack(s7);
	lt3.PushBack(s8);
	mp.Insert(std::pair<std::string, List<String>>("-1", lt1));
	mp.Insert(std::pair<std::string, List<String>>("0", lt2));
	mp.Insert(std::pair<std::string, List<String>>("1", lt3));
	ByteBuffer bb;
	bb.VarWrite(mp);
	for (size_t i = 0; i < bb.dataLen; ++i)
	{
		printf("%02X ", (unsigned char)bb.buf[i]);
	}
	cout << endl;
	bb.VarRead(mp1);
	for (auto it = mp1.Begin(); it != mp1.End(); ++it)
	{
		Console::Write("Key: ", it->first, "   Value: ");
		for (auto it1 = it->second.Begin(); it1 != it->second.End(); ++it1)
		{
			Console::Write((*it1).Std_str(), " ");
		}
		Console::WriteLine();
	}
	cout << "===================================================" << endl;
}

void Serialization_Test_4()
{
#if Ajy_BYTEBUFFER_PTR_SUPPORT_ENABLED
	HashMap<int, Vector<std::string>> hmp, hmp1;
	Vector<std::string> vec1, vec2, vec3;
	std::string s0("asdhf"), s1("dsafsad"), s2("dfgsdhf");
	std::string s3("java"), s4("ifdoisgn"), s5("212sdaf");
	std::string s6("-dsa6621"), s7("pldsfadf"), s8("cplusplus");
	vec1.Push(s0, s1, s2);
	vec2.Push(s3, s4, s5);
	vec3.Push(s6, s7, s8);
	hmp.Add(-1, vec1);
	hmp.Add(0, vec2);
	hmp.Add(1, vec3);
	ByteBuffer bb;
	bb.RootWrite(hmp);
	for (size_t i = 0; i < bb.dataLen; ++i)
	{
		printf("%02X ", (unsigned char)bb.buf[i]);
	}
	cout << endl;
	bb.RootRead(hmp1);
	auto data = hmp1.Data();
	for (int i = 0; i < data.DataLen(); ++i)
	{
		auto o = data[i];
		cout << "Key: " << o->key << "   Value: ";
		for (int j = 0; j < o->value.DataLen(); ++j)
		{
			cout << o->value[j] << " ";
		}
		cout << endl;
	}
	cout << "===================================================" << endl;
#endif
}

void Serialization_Test_5()
{
#if Ajy_BYTEBUFFER_PTR_SUPPORT_ENABLED
	HashMap<int, Vector<std::string>>* hmp = new HashMap<int, Vector<std::string>>();
	HashMap<int, Vector<std::string>>* hmp1 = nullptr;
	Vector<std::string> vec1, vec2, vec3;
	std::string s0("asdhf"), s1("dsafsad"), s2("dfgsdhf");
	std::string s3("java"), s4("ifdoisgn"), s5("212sdaf");
	std::string s6("-dsa6621"), s7("pldsfadf"), s8("cplusplus");
	vec1.Push(s0, s1, s2);
	vec2.Push(s3, s4, s5);
	vec3.Push(s6, s7, s8);
	hmp->Add(-1, vec1);
	hmp->Add(0, vec2);
	hmp->Add(1, vec3);
	ByteBuffer bb;
	bb.Write(hmp);
	for (size_t i = 0; i < bb.dataLen; ++i)
	{
		printf("%02X ", (unsigned char)bb.buf[i]);
	}
	cout << endl;
	bb.Read(hmp1);
	auto data = hmp1->Data();
	for (int i = 0; i < data.DataLen(); ++i)
	{
		auto o = data[i];
		cout << "Key: " << o->key << "   Value: ";
		for (int j = 0; j < o->value.DataLen(); ++j)
		{
			cout << o->value[j] << " ";
		}
		cout << endl;
	}
	cout << "===================================================" << endl;
#endif
}

void Serialization_Test_6()
{
	HashMap<float, Vector<std::string>> hmp, hmp1;
	Vector<std::string> vec1, vec2, vec3;
	std::string s0("asdhf"), s1("dsafsad"), s2("dfgsdhf");
	std::string s3("java"), s4("ifdoisgn"), s5("212sdaf");
	std::string s6("-dsa6621"), s7("pldsfadf"), s8("cplusplus");
	vec1.Push(s0, s1, s2);
	vec2.Push(s3, s4, s5);
	vec3.Push(s6, s7, s8);
	hmp.Add(0.123, vec1);
	hmp.Add(0.234, vec2);
	hmp.Add(0.345, vec3);
	ByteBuffer bb;
	bb.Write(hmp);
	for (size_t i = 0; i < bb.dataLen; ++i)
	{
		printf("%02X ", (unsigned char)bb.buf[i]);
	}
	cout << endl;
	bb.Read(hmp1);
	auto data = hmp1.Data();
	for (int i = 0; i < data.DataLen(); ++i)
	{
		auto o = data[i];
		cout << "Key: " << o->key << "   Value: ";
		for (int j = 0; j < o->value.DataLen(); ++j)
		{
			cout << o->value[j] << " ";
		}
		cout << endl;
	}
	cout << "===================================================" << endl;
}

void Serialization_Test_7()
{
	HashMap<double, List<std::string>> hmp, hmp1;
	List<std::string> vec1, vec2, vec3;
	std::string s0("asdhf你好"), s1("dsafsad"), s2("dfgsdhf");
	std::string s3("java"), s4("ifdoisgnsaj啥快递"), s5("212sdaf");
	std::string s6("-dsa6621"), s7("pldsfadf"), s8("cplus撒谎plus");
	vec1.PushBack(s0);
	vec1.PushBack(s1);
	vec1.PushBack(s2);
	vec2.PushBack(s3);
	vec2.PushBack(s4);
	vec2.PushBack(s5);
	vec3.PushBack(s6);
	vec3.PushBack(s7);
	vec3.PushBack(s8);
	hmp.Add(0.123456789, vec1);
	hmp.Add(0.0123456789, vec2);
	hmp.Add(0.00123456789, vec3);
	ByteBuffer bb;
	bb.Write(hmp);
	for (size_t i = 0; i < bb.dataLen; ++i)
	{
		printf("%02X ", (unsigned char)bb.buf[i]);
	}
	cout << endl;
	bb.Read(hmp1);
	auto data = hmp1.Data();
	for (int i = 0; i < data.DataLen(); ++i)
	{
		auto o = data[i];
		cout << "Key: " << o->key << "   Value: ";
		for (auto it = o->value.Begin(); it != o->value.End(); ++it)
		{
			cout << *it << " ";
		}
		cout << endl;
	}
	cout << "===================================================" << endl;
}

void Serialization_Test()
{
	Serialization_Test_1();
	Serialization_Test_2();
	Serialization_Test_3();
	//Serialization_Test_4();
	//Serialization_Test_5();
	Serialization_Test_6();
	Serialization_Test_7();
}
