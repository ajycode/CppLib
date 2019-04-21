#pragma once

//#include <experimental/generator>

template <typename T, typename ... Others>
struct _mr : _mr<Others...>
{
	T& value;
	typedef _mr<Others...> base_type;
	_mr(T& t, base_type& father)
		: value(t),
		base_type(father.value, static_cast<typename base_type::base_type&>(father))
	{
	}

	template <typename Arg>
	_mr<Arg, T, Others...> operator ,(Arg& arg)
	{
		return _mr<Arg, T, Others...>(arg, *this);
	}

	template <typename Container>
	void operator=(Container&& c)
	{
		auto it = c.begin();
		std::advance(it, sizeof...(Others));
		set(it);
	}

	template <typename Iterator>
	void set(Iterator& it)
	{
		value = *it--;
		base_type::set(it);
	}
};

template <typename T>
struct _mr<T>
{
	T& value;
	typedef _mr<T> base_type;
	_mr(T& t) : value(t) {}

	_mr(T& t, base_type&) : value(t) {}

	template <typename Arg>
	_mr<Arg, T> operator ,(Arg& arg)
	{
		return _mr<Arg, T>(arg, *this);
	}

	template <typename Container>
	void operator=(Container&& c)
	{
		auto it = c.begin();
		set(it);
	}

	template <typename Iterator>
	void set(Iterator& it)
	{
		value = *it;
	}
};

struct _mr0
{
	template <typename T>
	_mr<T> operator ,(T& t)
	{
		return _mr<T>(t);
	}
};

static _mr0 MR;

std::vector<int> foo()
{
	return{ 3, 4 };
}

struct A
{
	A(int _i) :i(_i) {}
	int i;
	bool operator<(const A& a) const
	{
		return i > a.i;
	}
};

class B
{
public:
	int i;
	void func() {}
};

class C :public B
{
public:
	int i;
	void func() {}
};

class D :public B, public C
{
public:
	int i;
	void func() {}
};


struct Base
{
	virtual ~Base() {};
};
template<class T>
struct Derive : public Base
{
	T value;
	Derive(T const& v) : value(v) {}
	Derive(T&& v) : value((T&&)v) {}
	Derive(Derive const& o) = default;
	Derive(Derive&& o) = default;
	Derive& operator=(Derive const& o) = default;
	Derive& operator=(Derive&& o) = default;
	~Derive() override {}
};

void func(std::string const& str)
{
	auto o = new Derive<std::string>(str);
}

void func(std::string&& str)
{
	auto o = new Derive<std::string>(std::move(str));
}

template<class T>
void func(T const& t)
{
	auto o = new Derive<T>(t);
}

template<class T>
void func(T&& t)
{
	auto o = new Derive<T>(std::move(t));
}

template<class T, int N>
std::function<T *()> makeArrayReader(T(&array)[N])
{
	int i = 0;
	return[&array, i]()mutable->T*
	{
		return i == N ? nullptr : &array[i++];
	};
}

template<class F, class... Args>
void expand(F const& f, Args&&...args)
{
	initializer_list<int>{(f(std::forward< Args>(args)), 0)...};
}

template<size_t N>
struct Fibonaci
{
	static size_t const value = Fibonaci<N - 1>::value + Fibonaci<N - 2>::value;
};

template<>
struct Fibonaci<1>
{
	static size_t const value = 1;
};

template<>
struct Fibonaci<0>
{
	static size_t const value = 0;
};

//std::experimental::generator<int> co(int n)
//{
//	int a = 0;
//	int b = 1;
//	while (n-->0)
//	{
//		yield a;
//		auto next = a + b;
//		a = b;
//		b = next;
//	}
//}

// catch-all case
void test(...)
{
	std::cout << "Couldn't call\n";
}

// catch when C is a reference-to-class type and F is a member function pointer
template<class C, class F>
auto test(C c, F f) -> decltype((c.*f)(), void()) // 'C' is reference type
{
	std::cout << "Could call on reference\n";
}

// VC++ didn't implement "Expression SFINAE" completely
// so can't run following code

//// catch when C is a pointer-to-class type and F is a member function pointer
//template<class C, class F>
//auto test(C c, F f) -> decltype((c->*f)(), void()) // 'C' is pointer type
//{
//	std::cout << "Could call on pointer\n";
//}

struct X {
	void f() {}
};

template<class T>
class XXX
{
public:
	XXX(int a);
	template<template<class,class> class U, class V, class W>
	operator U<V, W>();
private:
	int num;
};

template<class T>
XXX<T>::XXX(int a):num(a){}

template<class T>
template<template<class,class> class U,class V,class W>
XXX<T>::operator U<V, W>()
{
	cout << "U,V,W" << endl;
	cout << typeid(U).name() << endl;
	cout << typeid(V).name() << endl;
	cout << typeid(W).name() << endl;
	return U<V, W>(num);
}

void funcXXX(std::vector<int> t)
{
	for (int a: t)
	{
		cout << a << endl;
	}
}

// 变长模板参数类型同一性限制
template<class T, class U, class ...V>
struct Check
{
	static bool const value = Check<T, U>::value && Check<U, V...>::value;
};

template<class T>
struct Check<T, T> : public std::true_type {};

template<class T, class U>
struct Check<T, U> : public std::false_type {};

template<class T, class ...Args>
typename std::enable_if<Check<T, Args...>::value, void>::type xxx(Args&&... args)
{
}

template<class T, class ...Args>
typename std::enable_if<!Check<T, Args...>::value, void>::type xxx(Args&&... args)
{
}

template<class X, template<class> class Y, template<class, class> class T>
T<X, Y<X>> sf_vec_find_if(const T<X, Y<X>>& vec, Ajy::function<bool(X)> func)
{
	T<X, Y<X>> ret;
	for (auto t : vec)
		if (func(t))
			ret.push_back(t);
	return ret;
}

template<class X>
class sadjd
{
public:
	template<template<class> class Y, template<class, class> class T>
	T<X, Y<X>> sf_vec_find_if(const T<X, Y<X>>& vec, Ajy::function<bool(X)> func)
	{
		T<X, Y<X>> ret;
		for (auto t : vec)
			if (func(t))
				ret.push_back(t);
		return ret;
	}
};

void Other_Test()
{
	std::vector<int> v;
	v.emplace_back(1);
	v.emplace_back(2);
	v.emplace_back(3);
	v.emplace_back(4);
	v.emplace_back(5);
	//std::vector<int> v1 = sf_vec_find_if(v, [](int x)->bool {return x > 2; });	// 错误的用法,这样匹配不到func
	std::vector<int> v1 = sf_vec_find_if(v, Ajy::function<bool(int)>([](int x)->bool {return x > 2; }));
	for each (int var in v1)
	{
		std::cout << var << std::endl;
	}
	sadjd<int> sadjs_int;
	v1 = sadjs_int.sf_vec_find_if(v, [](int x)->bool {return x > 0; });	// 将func模板参数固定到类上，就可以直接传lambda了
	for each (int var in v1)
	{
		std::cout << var << std::endl;
	}

	// 变长模板参数类型同一性限制测试
	xxx<int>(1, 2, 3, 4);
	xxx<bool>(true, false, false, true);
	xxx<std::string>(std::string("1"), std::string("2"), std::string("3"), std::string("4"));
	xxx<char>('a', 'b', 'c', 'd');
	xxx<float>(1.1f, 1.2f, 1.3f, 1.4f);
	xxx<double>(1.1, 1.2, 1.3, 1.4);
	xxx<int>(1, true, 2.1, 1.3f, 'c');

	/*X x;
	test(x, &X::f);
	test(&x, &X::f);
	test(42, 1337);*/

	/*int a = 1, b = 2, c = 3;
	( MR, a, b, c ) = foo();
	cout << a << b << c << endl;*/

	/*D* d = new D();
	B* b = (B*)d;
	C* c = (C*)d;

	cout << (int)d - (int)c << endl;
	cout << (int)d - (int)b << endl;*/

	/*int n = 14999999;
	StopWatch sw;
	for (int i = 0; i <= n; ++i)
	{
		auto o = new RBTreeNode<std::pair<int, int>>;
		new (&o->val) std::pair<int, int>(i, i);
	}
	cout << sw() << endl;
	sw.Reset();
	MemoryPool pool;
	pool.Init<RBTreeNode<std::pair<int, int>>>(32);
	for (int i = 0; i <= n; ++i)
	{
		auto o = (RBTreeNode<std::pair<int, int>>*)pool.Alloc();
		new (&o->val) std::pair<int, int>(i, i);
	}
	cout << sw() << endl;
	sw.Reset();
	for (int i = 0; i <= n; ++i)
	{
		auto o = new RBTreeNode<std::string>;
		char buf[10];
		sprintf(buf, "%d", i);
		new (&o->val) std::string(buf);
	}
	cout << sw() << endl;
	sw.Reset();
	MemoryPool pool1;
	pool1.Init<RBTreeNode<std::string>>(32);
	for (int i = 0; i <= n; ++i)
	{
		auto o = (RBTreeNode<std::string>*)pool1.Alloc();
		char buf[10];
		sprintf(buf, "%d", i);
		new (&o->val) std::string(buf);
	}
	cout << sw() << endl;*/

	/*char const* a[] = { "装","逼","大","法","好","!" };
	auto next = makeArrayReader(a);
	for (auto it = next(); it != nullptr; it = next())
	{
		std::cout << *it;
	}
	cout << endl;*/

	/*XXX<int> xxx(10);
	funcXXX(xxx);*/

	expand([](auto i) {cout << i << endl; }, 1, 2.1, "test", 'c', false);

	/*auto o = Fibonaci<10>::value;
	Console::WriteLine(o);*/

	/*ByteBuffer bb;
	float f = 0.123456789f, f1;
	cout << f << endl;
	Console::WriteLine(f);
	bb.Write(f);
	bb.Read(f1);
	cout << f1 << endl;
	Console::WriteLine(f1);
	char buf[10];
	sprintf(buf, "%f", f);
	String s(buf);
	cout << s.Std_str() << endl;
	std::ostringstream buffer;
	buffer << f;
	cout << std::string(buffer.str()) << endl;
	Console::WriteLine(buffer.str());
	cout << (f == f1) << endl;*/

	/*bool c2b = std::is_convertible<C*, B*>::value;
	bool a2b = std::is_convertible<A*, B*>::value;
	bool b2c = std::is_convertible<B*, C*>::value;
	bool c2b1 = std::is_convertible<C, B>::value;
	bool a2b1 = std::is_convertible<A, B>::value;
	bool a2a = std::is_convertible<A, A>::value;

	std::cout << std::boolalpha;
	std::cout << c2b << '\n';
	std::cout << a2b << '\n';
	std::cout << b2c << '\n';
	std::cout << c2b1 << '\n';
	std::cout << a2b1 << '\n';
	std::cout << a2a << '\n';*/

}

