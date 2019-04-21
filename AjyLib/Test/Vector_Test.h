#pragma once

void Vector_Test_1()
{
	Vector<List<String>> vec;
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
	vec.Push(std::move(lt));
	//vec.Push(lt);
	auto& o = vec.Top();
	for (auto it = o.Begin(); it != o.End(); ++it)
	{
		cout << (*it).Std_str() << endl;
	}

	cout << "-------------------------------------------------------------" << endl;

	Vector<String> vec1;
	s.Append("a");
	s1.Push('b');
	vec1.Push(s);
	vec1.Push(s1);
	vec1.Push(s2);
	vec1.Push(s3);
	vec1.Push(s4);
	vec1.Push(s5);
	vec1.Push(s6);
	vec1.Push(s7);
	vec1.Push(s8);
	vec1.Push(s9);
	for (auto i = 0; i < vec1.DataLen(); ++i)
	{
		cout << vec1[i].Std_str() << endl;
	}

	cout << "-------------------------------------------------------------" << endl;

	Vector<std::string> vec2;
	vec2.Push(std::string("helloworld"));
	vec2.Push(std::string("helloworld"));
	vec2.Push(std::string("helloworld"));
	vec2.Push(std::string("helloworld"));
	vec2.Push(std::string("helloworld"));
	vec2.Push(std::string("helloworld"));
	vec2.Push(std::string("helloworld"));
	vec2.Push(std::string("helloworld"));
	vec2.Push(std::string("helloworld"));
	vec2.Push(std::string("helloworld"));
	for (auto i = 0; i < vec2.DataLen(); ++i)
	{
		cout << vec2[i] << endl;
	}

	cout << "-------------------------------------------------------------" << endl;
}

void Print_BitArray(Ajy::Vector<bool> const& bitArray)
{
	for (size_t i = 0; i < bitArray.DataLen(); ++i)
	{
		cout << bitArray[i] << " ";
	}
	cout << endl;
}

// Î»Êý×é²âÊÔ
void Vector_Test_2()
{
	Ajy::Vector<bool> bitArray;
	bitArray.Resize(8, false);
	Cout.WriteLine(bitArray.BufByteLen());
	Cout.WriteLine(bitArray.BufLen());
	Cout.WriteLine(bitArray.DataByteLen());
	Cout.WriteLine(bitArray.DataLen());
	Print_BitArray(bitArray);
	bitArray.Push(true, true, false, true);
	Print_BitArray(bitArray);
	bitArray.FillTrue();
	Print_BitArray(bitArray);
	bitArray.FillFalse();
	Print_BitArray(bitArray);
	bitArray.SetRange(0, true, true, false, true);
	Print_BitArray(bitArray);
	bitArray.Set(0, false);
	bitArray.Set(1, false);
	bitArray.Set(3, false);
	Print_BitArray(bitArray);
	bitArray.Resize(16);
	Print_BitArray(bitArray);
	bitArray.Fill(true, 0, 7);
	Print_BitArray(bitArray);
	bitArray.Fill(false, 3, 4);
	Print_BitArray(bitArray);
	Cout.WriteLine("--------------------------------------------------");

	bitArray.FillFalse();
	bitArray[1] = true;
	bool b = bitArray[1];
	bitArray[2] = bitArray[1];
	Print_BitArray(bitArray);
	bitArray[0] = bitArray[3] = bitArray[1];
	Print_BitArray(bitArray);
	bitArray[0] = bitArray[1] = bitArray[2] = bitArray[3] = false;
	Print_BitArray(bitArray);
	(bitArray[0] = bitArray[1]) = true;
	Print_BitArray(bitArray);
	(bitArray[0] = bitArray[1] = bitArray[0]) = false;
	Print_BitArray(bitArray);
	Cout.WriteLine("--------------------------------------------------");

	Ajy::Vector<bool> bitArray1(bitArray);
	Print_BitArray(bitArray);
	Print_BitArray(bitArray1);
	Cout.WriteLine("--------------------------------------------------");

	Ajy::Vector<bool> bitArray2(std::move(bitArray));
	Print_BitArray(bitArray);
	Print_BitArray(bitArray2);
	Cout.WriteLine("--------------------------------------------------");

	bitArray = bitArray1;
	Print_BitArray(bitArray);
	Print_BitArray(bitArray1);
	Cout.WriteLine("--------------------------------------------------");

	bitArray.Clear();
	Print_BitArray(bitArray);
	Cout.WriteLine("--------------------------------------------------");

	bitArray = std::move(bitArray1);
	Print_BitArray(bitArray);
	Print_BitArray(bitArray1);
	Cout.WriteLine("--------------------------------------------------");

	bitArray.FillTrue();
	bitArray.Push(true, true, true, true);
	Print_BitArray(bitArray);
	Cout.WriteLine("--------------------------------------------------");

	bitArray.Pop();
	bitArray.Pop();
	bitArray.Pop();
	bitArray.Pop();
	Print_BitArray(bitArray);
	Cout.WriteLine("--------------------------------------------------");
}

void Vector_Test()
{
	//Vector_Test_1();
	Vector_Test_2();
}
