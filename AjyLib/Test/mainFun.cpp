//#define _SCL_SECURE_NO_WARNINGS
//#define _CRT_SECURE_NO_WARNINGS

#include "All.h"
#ifdef Ajy_WINDOWS
	#include <windows.h>
#endif

#pragma pack(4)  
struct str
{
	int len;
	short c;
	char s[13];
	void xxxx() {}
	float xxxx(int i) { return 0; }
	float xxxx(int const& i) { return 0; }
	std::string BB(int i, float f, double d, char c) { return std::string(); }
};
#pragma pack(4)  
struct str1
{
	int len;
	short c;
	char* s;
	void xxxx() {}
	void xxxx(int i) {}
	float xxxx(int const& i) const { return 0; }
	std::string BB(int i, float f, double d) const { return std::string(); }
};

HAS_MEMBER_FUNCTION(HasFunc_xxxx, xxxx, void (T::*)())
HAS_MEMBER_FUNCTION(HasFunc_xxxx_int, xxxx, void (T::*)(int))
HAS_MEMBER_FUNCTION(HasFunc_xxxx_int_float, xxxx, float (T::*)(int))
HAS_MEMBER_FUNCTION(HasFunc_xxxx_int_constref_float, xxxx, float (T::*)(int const&))
HAS_MEMBER_FUNCTION(HasFunc_xxxx_int_constref_float_const, xxxx, float (T::*)(int const&) const)
HAS_MEMBER_FUNCTION(HasFunc_BB_1, BB, std::string (T::*)(int,float,double,char))
HAS_MEMBER_FUNCTION(HasFunc_BB_2, BB, std::string(T::*)(int, float, double) const)

void test2(int &a, int b)
{
	cout << a << endl;
	cout << b << endl;
}

template<class A, class B>
void test(A&& a, B&& b)
{
	test2(std::forward<A>(a), std::forward<B>(b));
}

int main()
{
	Ajy::cahash_set<string> st;
	st.emplace("1");
	st.emplace("2");
	st.emplace("3");
	st.emplace("4");
	Ajy::cahash_set<string> st1(std::move(st), std::allocator<string>());
	/*st1.erase("1");
	st1.erase("2");
	st1.erase("3");
	st1.erase("4");*/
	for each (string var in st)
	{
		Console::WriteLine(var);
	}
	for (string var : st1)
	{
		Console::WriteLine(var);
	}
	std::set<int> ss;

	cout << HasFunc_xxxx<str>::value << endl;
	cout << HasFunc_xxxx<str1>::value << endl;
	cout << HasFunc_xxxx_int<str>::value << endl;
	cout << HasFunc_xxxx_int<str1>::value << endl;
	cout << HasFunc_xxxx_int_float<str>::value << endl;
	cout << HasFunc_xxxx_int_float<str1>::value << endl;
	cout << HasFunc_xxxx_int_constref_float<str>::value << endl;
	cout << HasFunc_xxxx_int_constref_float<str1>::value << endl;
	cout << HasFunc_xxxx_int_constref_float_const<str>::value << endl;
	cout << HasFunc_xxxx_int_constref_float_const<str1>::value << endl;
	cout << HasFunc_BB_1<str>::value << endl;
	cout << HasFunc_BB_1<str1>::value << endl;
	cout << HasFunc_BB_2<str>::value << endl;
	cout << HasFunc_BB_2<str1>::value << endl;
	
	/************************************************************************/
	/* test                                                                 */
	/************************************************************************/
	
	//MemoryPool_Test();
	//Map_Test();
	//MultiMap_test();
	//Set_Test();
	//MultiSet_Test();
	//List_Test();
	//HashMap_Test();
	//HashSet_Test();
	//String_Test();
	//Vector_Test();
	//Stack_Test();
	//Queue_Test();
	//Variant_Test();
	//PriorityQueue_Test();
	//Serialization_Test();
	//Dictionary_Test();
	//MakeIndexSequence_Test();
	//SkipList_Test();
	//CMHashMap_Test();
	//CMHashSet_Test();
	//AnyTest();
	//SharedPtr_Test();
	//RedBlackTree_Test();
	//CacheAffinityHash_Test();
	//SizeBalanceTree_Test();
	//BppTree_Test();
	//AVLTree_Test();
	//Timer_Test();
	CheckMember_Test();
	//Traits_Test();
	//Tuple_Test();
	//TypeList_Test();
	//Function_Test();
	//Bind_Test();
	
	

	/************************************************************************/
	/* compare                                                              */
	/************************************************************************/

	//Vector_Compare();
	//List_Compare();
	//HashMap_Compare();
	//HashMultiMap_Compare();
	//HashSet_Compare();
	//HashMultiSet_Compare();
	//Map_Compare();
	//Multimap_Compare();
	//Set_Compare();
	//MultiSet_Compare();


	/************************************************************************/
	/* other                                                                */
	/************************************************************************/

	//Other_Test();
	
	/*cout << sizeof(str) << endl;
	cout << sizeof(str1) << endl;
	str* pt = nullptr;
	printf("&i = %x\n", &pt->s);
	int offset = offsetof(str, s);
	cout << offset << endl;
	int offset1 = offsetof(str1, s);
	cout << offset1 << endl;*/

 	return 0;
}
