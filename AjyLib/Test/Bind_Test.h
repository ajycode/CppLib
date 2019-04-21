#pragma once

namespace BindTest
{

	double add(double a, double b)
	{
		return a + b;
	}

	int sub(int& a, int const& b)
	{
		return a - b;
	}

	char const* test(std::string& str)
	{
		return str.c_str();
	}

	void test1()
	{
		cout << "BindTest::test1" << endl;
	}

	class A
	{
	public:
		static void Test(bool b)
		{
			cout << "BindTest::A::Test " << b << endl;
		}
		int Add(int a, int b)
		{
			return a + b + i;
		}
		int i = -1;
	};

	void testA1(A* a, int i)
	{
		a->i = i;
	}

	void testA2(A& a, int i = 0)
	{
		a.i = i;
	}

	class B
	{
	public:
		int Sub(int a, int b)
		{
			return a - b;
		}
	};

	class testFunctor
	{
	public:
		int operator()(int x, int y) {
			return x * y;
		}
	};

}

void Bind_Test()
{

	cout << "=======================================================Test=============================================" << endl;

	auto bd1 = Bind(BindTest::add, 1.0, 2.0);
	cout << bd1() << endl;

	auto bd2 = Bind(BindTest::sub, 2, 1);
	cout << bd2() << endl;

	auto bd3 = Bind(BindTest::sub, BindDetail::_2, BindDetail::_1);
	cout << bd3(1, 2, 3) << endl;

	auto bd4 = Bind(BindTest::sub, BindDetail::_1, 1);
	cout << bd4(2) << endl;

	auto bd5 = Bind(BindTest::sub, 2, BindDetail::_1);
	cout << bd5(1) << endl;

	auto bd6 = Bind(BindTest::test, std::string("helloworld"));
	cout << bd6() << endl;

	auto bd7 = Bind(BindTest::test1);
	bd7();

	auto bd8 = Bind(BindTest::A::Test, true);
	bd8();

	BindTest::A a;
	auto bd9 = Bind(&BindTest::A::Add, &a, 3, 1);
	cout << bd9() << endl;

	BindTest::A a1;
	a1.i = -2;
	auto bd10 = Bind(&BindTest::A::Add, a1, 1, 3);
	cout << bd10() << endl;

	auto bd11 = Bind(BindTest::testA1, BindDetail::_1, BindDetail::_2);
	bd11(&a1, 0);
	cout << a1.i << endl;
	auto bd12 = Bind(BindTest::testA2, a1, BindDetail::_1);
	bd12(1);
	cout << a1.i << endl;	// 因为对a1进行绑定时传进去的其实是拷贝，所以不会修改原值，必须在进行调用时传a1进去，(std::bind与此类似)
	auto bd13 = std::bind(BindTest::testA2, a1, std::placeholders::_1);
	bd13(1);
	cout << a1.i << endl;	// 因为对a1进行绑定时传进去的其实是拷贝，所以不会修改原值，必须在进行调用时传a1进去
	auto bd14 = Bind(BindTest::testA2, BindDetail::_1, BindDetail::_2);
	bd14(a1, 1);
	cout << a1.i << endl;
	auto bd15 = Bind(BindTest::testA1, &a1, BindDetail::_1);
	bd15(0);
	cout << a1.i << endl;

	auto bd16 = Bind(BindTest::testFunctor(), 2, 5);
	cout << bd16() << endl;
	auto bd17 = Bind(BindTest::testFunctor(), BindDetail::_1, 5);
	cout << bd17(4, 10) << endl;

	cout << "======================================================" << endl;

	std::function<double(double, double)> func = Bind(BindTest::add, BindDetail::_1, BindDetail::_2);
	cout << func(5.0, 6.0) << endl;

	BindTest::A a2;
	a2.i = -2;
	std::function<int(int, int)> func1 = Bind(&BindTest::A::Add, a2, BindDetail::_1, BindDetail::_2);
	cout << func1(1, 1) << endl;

	BindTest::A* a3 = new BindTest::A();
	a3->i = -10;
	std::function<int(int, int)> func2 = Bind(&BindTest::A::Add, a3, BindDetail::_1, BindDetail::_2);
	cout << func2(5, 1) << endl;

	func1 = Bind(&BindTest::A::Add, a3, BindDetail::_1, BindDetail::_2);
	cout << func1(1, 1) << endl;

	cout << "=====================================================Compare============================================" << endl;

	cout << "============================MyBind==========================" << endl;

	BindTest::B b;
	func2 = Bind(&BindTest::B::Sub, b, BindDetail::_1, BindDetail::_2);
	cout << func2(5, 1) << endl;
	func2 = Bind(&BindTest::B::Sub, b, BindDetail::_2, BindDetail::_1);
	cout << func2(5, 1) << endl;
	func2 = Bind(&BindTest::B::Sub, b, BindDetail::_1, 2);
	cout << func2(5, 1) << endl;
	func2 = Bind(&BindTest::B::Sub, b, 2, BindDetail::_2);
	cout << func2(5, 1) << endl;
	func2 = Bind(&BindTest::B::Sub, b, 20, 10);
	cout << func2(5, 1) << endl;
	func2 = Bind([](int a, int b) {return a - b; }, 20, BindDetail::_1);
	cout << func2(5, 1) << endl;
	cout << "=============================std::bind=========================" << endl;
	func2 = std::bind(&BindTest::B::Sub, b, std::placeholders::_1, std::placeholders::_2);
	cout << func2(5, 1) << endl;
	func2 = std::bind(&BindTest::B::Sub, b, std::placeholders::_2, std::placeholders::_1);
	cout << func2(5, 1) << endl;
	func2 = std::bind(&BindTest::B::Sub, b, std::placeholders::_1, 2);
	cout << func2(5, 1) << endl;
	func2 = std::bind(&BindTest::B::Sub, b, 2, std::placeholders::_2);
	cout << func2(5, 1) << endl;
	func2 = std::bind(&BindTest::B::Sub, b, 20, 10);
	cout << func2(5, 1) << endl;
	//std::function<void(void)> func3 = std::bind(&BindTest::B::Sub, b, 20, 10);
	Ajy::function<int(void)> func3 = Ajy::Bind(&BindTest::B::Sub, b, 20, 10);
	func3();
	//cout << func3() << endl;
	func2 = std::bind([](int a, int b) {return a - b; }, 20, std::placeholders::_1);
	cout << func2(5, 1) << endl;
}

