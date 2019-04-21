#pragma once

namespace TypeListTest
{
	using types_t = Ajy::TypeList::types<short, int, unsigned char, long long, float&, double const, long*>;

	template <class T, class U>
	struct is_large
		: std::integral_constant<bool, (sizeof(T) > sizeof(U)) >
	{};

	template<class Type>
	typename std::enable_if<!Ajy::TypeList::is_types<Type>::value, void>::type printType()
	{
		Cout.WriteLine(typeid(Type).name());
	}
	template<class Type>
	typename std::enable_if<Ajy::TypeList::is_types<Type>::value, void>::type printType()
	{
		print(Type());
	}
	template<size_t I = 0, class TypeList>
	typename std::enable_if<I == Ajy::TypeList::types_size<TypeList>::value, void>::type print(TypeList const& t)
	{
		Cout.WriteLine();
	}
	template<size_t I = 0, class TypeList>
	typename std::enable_if<I < Ajy::TypeList::types_size<TypeList>::value, void>::type print(TypeList const& t)
	{
		Cout.Write(typeid(typename Ajy::TypeList::types_at<TypeList, I>::type).name(),",");
		//printType<typename Ajy::TypeList::types_at<TypeList, I>::type>();
		print<I + 1>(t);
	}

	void test_types_at()
	{
		Cout.WriteLine("---------------------------------------test_types_at---------------------------------------");
		printType<types_t>();
		{
			using t_t = Ajy::TypeList::types_front_t<types_t>;
			printType<t_t>();
		}
		{
			using t_t = Ajy::TypeList::types_back_t<types_t>;
			printType<t_t>();
		}
		{
			using t_t = Ajy::TypeList::types_at_t<types_t, 3>;
			printType<t_t>();
		}
		{
			//using t_t = Ajy::TypeList::types_at_t<types_t, types_size<types_t>::value>;
			//printType<t_t>();
		}
		{
			//using t_t = Ajy::TypeList::types_at_t<int, 3>;
			//printType<t_t>();
		}
	}

	void test_types_link()
	{
		Cout.WriteLine("---------------------------------------test_types_link---------------------------------------");
		printType<types_t>();
		{
			using t_t = Ajy::TypeList::types_link_t<void, types_t>;
			printType<t_t>();
		}
		{
			using t_t = Ajy::TypeList::types_link_t<types_t, void>;
			printType<t_t>();
		}
		{
			using t_t = Ajy::TypeList::types_link_t<types_t, types_t>;
			printType<t_t>();
		}
	}

	void test_types_assign()
	{
		Cout.WriteLine("---------------------------------------test_types_assign---------------------------------------");
		printType<types_t>();
		{
			using t_t = Ajy::TypeList::types_assign_t<3, int>;
			printType<t_t>();
		}
		{
			using t_t = Ajy::TypeList::types_assign_t<2, types_t>;
			printType<t_t>();
		}
		{
			using t_t = Ajy::TypeList::types_assign_t<0, types_t>;
			printType<t_t>();
		}
	}

	void test_types_insert()
	{
		Cout.WriteLine("---------------------------------------test_types_insert---------------------------------------");
		printType<types_t>();
		{
			using t_t = Ajy::TypeList::types_insert_t<types_t, 0, void>;
			printType<t_t>();
		}
		{
			using t_t = Ajy::TypeList::types_insert_t<types_t, Ajy::TypeList::types_size<types_t>::value, void>;
			printType<t_t>();
		}
		{
			using t_t = Ajy::TypeList::types_insert_t<types_t, 3, void>;
			printType<t_t>();
		}
		{
			using t_t = Ajy::TypeList::types_insert_t<types_t, 2, Ajy::TypeList::types<void, void*, void**>>;
			printType<t_t>();
		}
		{
			//using t_t = Ajy::TypeList::types_insert_t<types_t, Ajy::TypeList::types_size<types_t>::value + 1, void>;
			//printType<t_t>();
		}
		{
			//using t_t = Ajy::TypeList::types_insert_t<int, 1, void>;
			//printType<t_t>();
		}
	}

	void test_types_erase()
	{
		Cout.WriteLine("---------------------------------------test_types_erase---------------------------------------");
		printType<types_t>();
		{
			using t_t = Ajy::TypeList::types_erase_t<types_t, 0>;
			printType<t_t>();
		}
		{
			using t_t = Ajy::TypeList::types_erase_t<types_t, 2, 3>;
			printType<t_t>();
		}
		{
			using t_t = Ajy::TypeList::types_erase_t<types_t, Ajy::TypeList::types_size<types_t>::value - 1>;
			printType<t_t>();
		}
		{
			//using t_t = Ajy::TypeList::types_erase_t<types_t, -1, 0>;
			//printType<t_t>();
		}
		{
			//using t_t = Ajy::TypeList::types_erase_t<types_t, Ajy::TypeList::types_size<types_t>::value>;
			//printType<t_t>();
		}
		{
			//using t_t = Ajy::TypeList::types_erase_t<Ajy::TypeList::types<void>, 0, 3>;
			//printType<t_t>();
		}
		{
			//using t_t = Ajy::TypeList::types_erase_t<Ajy::TypeList::types<void>, 0, 0>;
			//printType<t_t>();
		}
		{
			//using t_t = Ajy::TypeList::types_erase_t<Ajy::TypeList::types<>, 0, 3>;
			//printType<t_t>();
		}
		{
			//using t_t = Ajy::TypeList::types_erase_t<int, 0>;
			//printType<t_t>();
		}
	}

	void test_types_find()
	{
		Cout.WriteLine("---------------------------------------test_types_find---------------------------------------");
		std::cout << Ajy::TypeList::types_find<types_t, long long>::value << std::endl;
		std::cout << Ajy::TypeList::types_find<types_t, void>::value << std::endl;
	}

	void test_types_replace()
	{
		Cout.WriteLine("---------------------------------------test_types_replace---------------------------------------");
		printType<types_t>();
		{
			using t_t = Ajy::TypeList::types_replace_t<types_t, int, void*&&>;
			printType<t_t>();
		}
		{
			using t_t = Ajy::TypeList::types_replace_t<Ajy::TypeList::types<int, long, int>, int, void>;
			printType<t_t>();
		}
		{
			using t_t = Ajy::TypeList::types_replace_t<Ajy::TypeList::types<int, long, int>, int, Ajy::TypeList::types<void, void>>;
			printType<t_t>();
		}
	}

	void test_types_remove()
	{
		Cout.WriteLine("---------------------------------------test_types_remove---------------------------------------");
		printType<types_t>();
		{
			using t_t = Ajy::TypeList::types_remove_t<Ajy::TypeList::types_link_t<types_t, types_t>, int>;
			printType<t_t>();
		}
		{
			using t_t = Ajy::TypeList::types_remove_t<Ajy::TypeList::types<int, int, int>, int>;
			printType<t_t>();
		}
		{
			using t_t = Ajy::TypeList::types_remove_t<Ajy::TypeList::types<void, void, void>, int>;
			printType<t_t>();
		}
		{
			using t_t = Ajy::TypeList::types_remove_t<types_t, Ajy::TypeList::types<int, long*, void>>;
			printType<t_t>();
		}
		{
			using t_t = Ajy::TypeList::types_remove_t<types_t, types_t>;
			printType<t_t>();
		}
		{
			using t_t = Ajy::TypeList::types_remove_t<Ajy::TypeList::types<Ajy::TypeList::types<>>, Ajy::TypeList::types<>>;
			printType<t_t>();
		}
		{
			//using t_t = Ajy::TypeList::types_remove_t<int, void>;
			//printType<t_t>();
		}
	}

	void test_types_compact()
	{
		Cout.WriteLine("---------------------------------------test_types_compact---------------------------------------");
		printType<types_t>();
		{
			using t_t = Ajy::TypeList::types_compact_t<Ajy::TypeList::types_link_t<types_t, types_t>>;
			printType<t_t>();
		}
		{
			using t_t = Ajy::TypeList::types_compact_t<Ajy::TypeList::types<int, int, void, void*, void>>;
			printType<t_t>();
		}
		{
			//using t_t = Ajy::TypeList::types_compact_t<int>;
			//printType<t_t>();
		}
	}

	void test_types_reverse()
	{
		Cout.WriteLine("---------------------------------------test_types_reverse---------------------------------------");
		printType<types_t>();
		{
			using t_t = Ajy::TypeList::types_reverse_t<types_t>;
			printType<t_t>();
		}
	}

	void test_types_select_if()
	{
		Cout.WriteLine("---------------------------------------test_types_select_if---------------------------------------");
		printType<types_t>();
		{
			using t_t = Ajy::TypeList::types_select_if_t<types_t, is_large>;
			printType<t_t>();
		}
	}

	void test_types_sort_if()
	{
		Cout.WriteLine("---------------------------------------test_types_sort_if---------------------------------------");
		printType<types_t>();
		{
			using t_t = Ajy::TypeList::types_sort_if_t<types_t, is_large>;
			printType<t_t>();
		}
		{
			using t_t = Ajy::TypeList::types_sort_if_t<Ajy::TypeList::types<int>, is_large>;
			printType<t_t>();
		}
	}
}

void TypeList_Test()
{
	TypeListTest::test_types_at();
	TypeListTest::test_types_link();
	TypeListTest::test_types_assign();
	TypeListTest::test_types_insert();
	TypeListTest::test_types_erase();
	TypeListTest::test_types_find();
	TypeListTest::test_types_replace();
	TypeListTest::test_types_remove();
	TypeListTest::test_types_compact();
	TypeListTest::test_types_reverse();
	TypeListTest::test_types_select_if();
	TypeListTest::test_types_sort_if();
}
