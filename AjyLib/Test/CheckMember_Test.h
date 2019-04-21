#pragma once

HAS_MEMBER_FUNCTION(HasFunc_Test, Test, void (T::*)())
HAS_MEMBER_FUNCTION(HasFunc_Test_i, Test, void (T::*)(int))
HAS_MEMBER_FUNCTION(HasFunc_Test_ir, Test, void (T::*)(int&))
HAS_MEMBER_FUNCTION(HasFunc_Test_s, Test, void (T::*)(std::string))
HAS_MEMBER_FUNCTION(HasFunc_Test_sr, Test, void (T::*)(std::string&))
HAS_MEMBER_FUNCTION(HasFunc_Test_scr, Test, void (T::*)(std::string const&))
HAS_MEMBER_FUNCTION(HasFunc_Test_srr, Test, void (T::*)(std::string&&))
HAS_MEMBER_FUNCTION(HasFunc_Test_ir_scr_srr, Test, void (T::*)(int&, std::string const&, std::string&&))
HAS_FUNCTION_MEMBER(Test)				// 对于只是因CV符修饰不同而导致的重载行为的检测存在问题
HAS_MEMBER_TYPE(HasType_test, test)
HAS_TYPE_MEMBER(test)
struct CheckMember_Test_1
{
	typedef int test;
	void Test() {}
	void Test(int i) {}
	void Test(int& i) {}
	void Test(std::string s) {}
	void Test(std::string& s) {}
	void Test(std::string const& s) {}
	void Test(std::string&& s) {}
	void Test(int& i, std::string const& s, std::string&& ss) {}
};
struct CheckMember_Test_2
{
};
struct CheckMember_Test_3
{
	typedef int test;
};
struct CheckMember_Test_4
{
	static void Test() {}
};
struct CheckMember_Test_5
{
	int Test() { return 0; }
};
struct CheckMember_Test_6
{
	void Test() const {}
};

class Testt
{
public:
	void func(std::string s){}
	void func(std::string& s) {}
	//void func(std::string const& s) {}
	void func(std::string&& s) {}
	int func(int& i, std::string const& s, std::string&& ss) { return 0; }
	char func(int& i, std::string const& s) const { return 'c'; }
};

HAS_MEMBER_FUNCTION_V3(func)


void CheckMember_Test()
{
	cout << std::boolalpha;

	cout << HAS_MEMBER_FUNCTION_func<Testt, false, void, string>::value << endl;
	cout << HAS_MEMBER_FUNCTION_func<Testt, false, void, string&>::value << endl;
	cout << HAS_MEMBER_FUNCTION_func<Testt, false, void, string const&>::value << endl;
	cout << HAS_MEMBER_FUNCTION_func<Testt, false, void, string&&>::value << endl;
	cout << HAS_MEMBER_FUNCTION_func<Testt, false, int, int&, string const&, string&&>::value << endl;
	cout << HAS_MEMBER_FUNCTION_func<Testt, true, int, int&, string const&, string&&>::value << endl;
	cout << HAS_MEMBER_FUNCTION_func<Testt, false, char, int&, string const&>::value << endl;
	cout << HAS_MEMBER_FUNCTION_func<Testt, true, char, int&, string const&>::value << endl;
	cout << "--------------------------" << endl;

	cout << HasFunc_Test<CheckMember_Test_1>::value << endl;
	cout << Has_MemberFunc_Test<CheckMember_Test_1>::value << endl;
	cout << Has_MemberFunc_Test<CheckMember_Test_1, void>::value << endl;
	cout << "--------------------------" << endl;
	cout << HasFunc_Test_i<CheckMember_Test_1>::value << endl;
	cout << HasFunc_Test_ir<CheckMember_Test_1>::value << endl;
	cout << HasFunc_Test_s<CheckMember_Test_1>::value << endl;
	cout << HasFunc_Test_sr<CheckMember_Test_1>::value << endl;
	cout << HasFunc_Test_scr<CheckMember_Test_1>::value << endl;
	cout << HasFunc_Test_srr<CheckMember_Test_1>::value << endl;
	cout << HasFunc_Test_ir_scr_srr<CheckMember_Test_1>::value << endl;
	cout << "--------------------------" << endl;
	cout << Has_MemberFunc_Test<CheckMember_Test_1, int>::value << endl;
	cout << Has_MemberFunc_Test<CheckMember_Test_1, int&>::value << endl;
	cout << Has_MemberFunc_Test<CheckMember_Test_1, std::string>::value << endl;
	cout << Has_MemberFunc_Test<CheckMember_Test_1, std::string&>::value << endl;
	cout << Has_MemberFunc_Test<CheckMember_Test_1, std::string const&>::value << endl;
	cout << Has_MemberFunc_Test<CheckMember_Test_1, std::string&&>::value << endl;
	cout << Has_MemberFunc_Test<CheckMember_Test_1, int&, std::string const&, std::string&&>::value << endl;
	cout << Has_MemberFunc_Test<CheckMember_Test_1, int&, std::string&, std::string&&>::value << endl;
	cout << Has_MemberFunc_Test<CheckMember_Test_1, double&, std::string&, std::string&&>::value << endl;
	cout << "--------------------------" << endl;
	cout << HasType_test<CheckMember_Test_1>::value << endl;
	cout << has_type_test<CheckMember_Test_1>::value << endl;
	cout << "=============================================================" << endl;

	cout << HasFunc_Test<CheckMember_Test_2>::value << endl;
	cout << HasType_test<CheckMember_Test_2>::value << endl;
	cout << has_type_test<CheckMember_Test_2>::value << endl;
	cout << "=============================================================" << endl;

	cout << HasFunc_Test<CheckMember_Test_3>::value << endl;
	cout << HasType_test<CheckMember_Test_3>::value << endl;
	cout << has_type_test<CheckMember_Test_3>::value << endl;
	cout << "=============================================================" << endl;

	cout << HasFunc_Test<CheckMember_Test_4>::value << endl;
	cout << HasType_test<CheckMember_Test_4>::value << endl;
	cout << has_type_test<CheckMember_Test_4>::value << endl;
	cout << "=============================================================" << endl;

	cout << HasFunc_Test<CheckMember_Test_5>::value << endl;
	cout << HasType_test<CheckMember_Test_5>::value << endl;
	cout << has_type_test<CheckMember_Test_5>::value << endl;
	cout << "=============================================================" << endl;

	cout << HasFunc_Test<CheckMember_Test_6>::value << endl;
	cout << HasType_test<CheckMember_Test_6>::value << endl;
	cout << has_type_test<CheckMember_Test_6>::value << endl;
}
