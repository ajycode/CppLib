#pragma once

namespace FunctionTest
{

	double add(double a, double b)
	{
		return a + b;
	}

	class A
	{
	public:
		static double add(double a, double b)
		{
			return a + b;
		}
		double sub(double a, double b)
		{
			return a - b + db;
		}
		double db = 1.23;
	};

	void test(A* a, double db)
	{
		a->db = db;
	}

	void test1(A& a, double db = 0.0)
	{
		a.db = db;
	}

	void test2(A a, double db = 0.0)
	{
		a.db = db;
	}

	class testFunctor
	{
	public:
		int operator()(int x, int y) {
			return x + y;
		}
	};

	int multi(int a, int b)
	{
		return a*b;
	}

}

void Function_Test()
{

	Ajy::function<double(double, double)> func = FunctionTest::add;
	cout << func(1.0, 1.0) << endl;

	FunctionTest::A a;
	Ajy::function<double(double, double)> func1 = Ajy::Bind(&FunctionTest::A::sub, a, Ajy::BindDetail::_1, Ajy::BindDetail::_2);
	cout << func1(2.0, 2.0) << endl;
	func1 = Ajy::Bind(&FunctionTest::A::sub, a, Ajy::BindDetail::_1, 3.0);
	cout << func1(2.0, 2.0) << endl;
	func1 = Ajy::Bind(&FunctionTest::A::sub, a, 3.0, Ajy::BindDetail::_1);
	cout << func1(2.0, 2.0) << endl;
	func1 = Ajy::Bind(&FunctionTest::A::sub, &a, 3.0, 1.0);
	cout << func1(2.0, 2.0) << endl;
	func1 = std::bind(&FunctionTest::A::sub, a, std::placeholders::_1, std::placeholders::_2);
	cout << func1(2.0, 2.0) << endl;
	Ajy::function<double(FunctionTest::A&, double, double)> func2 = &FunctionTest::A::sub;
	cout << func2(a, 2.0, 2.0) << endl;

	func1 = func;
	cout << func1(2.0, 2.0) << endl;

	Ajy::function<void(FunctionTest::A*, double)> func3 = FunctionTest::test;
	func3(&a, 0.0);
	cout << a.db << endl;
	Ajy::function<void(FunctionTest::A&, double)> func4 = FunctionTest::test1;
	func4(a, 1.0);
	cout << a.db << endl;
	Ajy::function<void(FunctionTest::A, double)> func5 = FunctionTest::test2;
	func5(a, 2.0);
	cout << a.db << endl;
	Ajy::function<int(int, int)> func6 = FunctionTest::multi;
	cout << func6(2, 5) << endl;
	func6 = FunctionTest::testFunctor();
	cout << func6(4, 6) << endl;
	func6 = [](int x, int y)->int {return x - y; };
	cout << func6(14, 4) << endl;

}
