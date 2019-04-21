#pragma once

void String_Test()
{
	std::string s("CPlusPlus");
	String s1("Java");
	std::cout << s1.Std_str() << std::endl;
	String str = String::MakeString("helloworld,", 1, ",", -0.123456789, ",", s, ",", s1, ",");
	str.Append(-1, ",", s.c_str(), ",", false, ",", true, ",");
	str.Push('a');
	str.AppendHex(31);
	std::cout << str.Std_str() << endl;


	String formatStr = Ajy::String::MakeFormatString("hello{0}[h]{1}{0}", ",", "world");
	formatStr.AppendFormat("{1}{0}{0}{2}{3}", 0, 1, 8, 6);
	Console::WriteLine(formatStr);
	Cout.WriteLineFormat("my telephone number is {1}{0}{0}{2}{3} !!!", 0, 1, 8, 6);


	String str1;
	char* ptr = "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	str1.Dump(ptr, 72);
	std::cout << str1.Std_str() << endl;
	String substr = str.Substr(0, 10);
	std::cout << substr.Std_str() << endl;
	String str2("0-11-222-333-4444-55555-666666-7777777-88888888-999999999");
	Vector<String> strvec = str2.Split('-');
	cout << "strvec.size:   " << strvec.DataLen() << endl;
	for (size_t i = 0; i < strvec.DataLen();++i)
	{
		cout << strvec[i].C_str() << endl;
	}
	cout << "strvec.size:   " << strvec.DataLen() << endl;
	String str3("0123456789");
	str3.Erase(0);
	std::cout << str3.C_str() << endl;
	String str4("BBC ABCDAB ABCDABCDABDE");
	std::pair<size_t, Vector<size_t>> findRes1 = str4.FindAllByKMP("ABCDABD", 0, 6);
	cout << "findRes1.first:   " << findRes1.first << endl;
	cout << "findRes1.second:   " << endl;
	for (size_t i = 0; i < findRes1.second.DataLen(); ++i)
	{
		cout << findRes1.second[i] << endl;
	}
	String str5("abcdefgdhsjabcdgjhabcdefokjkkdaABCDEddkfabcdeabcdabcde");
	std::pair<size_t, Vector<size_t>> findRes2 = str5.FindAllByKMP("abcde", 0, 6);
	cout << "findRes2.first:   " << findRes2.first << endl;
	cout << "findRes2.second:   " << endl;
	for (size_t i = 0; i < findRes2.second.DataLen(); ++i)
	{
		cout << findRes2.second[i] << endl;
	}
	String str6("BBC ABCDAB ABCDABCDABDE");
	std::pair<size_t, Vector<size_t>> findRes3 = str6.FindAllByBM("ABCDABD", 0, 6);
	cout << "findRes3.first:   " << findRes3.first << endl;
	cout << "findRes3.second:   " << endl;
	for (size_t i = 0; i < findRes3.second.DataLen(); ++i)
	{
		cout << findRes3.second[i] << endl;
	}
	String str7("abcdefgdhsjabcdgjhabcdefokjkkdaABCDEddkfabcdeabcdabcde");
	std::pair<size_t, Vector<size_t>> findRes4 = str7.FindAllByBM("abcde", 0, 6);
	cout << "findRes4.first:   " << findRes4.first << endl;
	cout << "findRes4.second:   " << endl;
	for (size_t i = 0; i < findRes4.second.DataLen(); ++i)
	{
		cout << findRes4.second[i] << endl;
	}
	String str8("BBC ABCDAB ABCDABCDABDE");
	std::pair<size_t, Vector<size_t>> findRes5 = str8.FindAllBySunday("ABCDABD", 0, 6);
	cout << "findRes5.first:   " << findRes5.first << endl;
	cout << "findRes5.second:   " << endl;
	for (size_t i = 0; i < findRes5.second.DataLen(); ++i)
	{
		cout << findRes5.second[i] << endl;
	}
	String str9("abcdefgdhsjabcdgjhabcdefokjkkdaABCDEddkfabcdeabcdabcde");
	std::pair<size_t, Vector<size_t>> findRes6 = str9.FindAllBySunday("abcde", 0, 6);
	cout << "findRes6.first:   " << findRes4.first << endl;
	cout << "findRes6.second:   " << endl;
	for (size_t i = 0; i < findRes6.second.DataLen(); ++i)
	{
		cout << findRes6.second[i] << endl;
	}
	String str10("BBC ABCDAB ABCDABCDABDE");
	std::pair<size_t, Vector<size_t>> findRes7 = str10.FindAllBySunday("ABCDABD", 0, 6);
	cout << "findRes7.first:   " << findRes7.first << endl;
	cout << "findRes7.second:   " << endl;
	for (size_t i = 0; i < findRes7.second.DataLen(); ++i)
	{
		cout << findRes7.second[i] << endl;
	}
	String str11("abcdefgdhsjabcdgjhabcdefokjkkdaABCDEddkfabcdeabcdabcde");
	std::pair<size_t, Vector<size_t>> findRes8 = str11.FindAllBySunday("abcde", 0, 6);
	cout << "findRes8.first:   " << findRes8.first << endl;
	cout << "findRes8.second:   " << endl;
	for (size_t i = 0; i < findRes8.second.DataLen(); ++i)
	{
		cout << findRes8.second[i] << endl;
	}
}
