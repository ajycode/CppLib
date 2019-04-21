#pragma once

namespace Ajy
{
	// todo : Tuple_Size支持， MSVC下natvis调试视图支持

	template<class... AllTypes>
	struct Tuple;

	template<> struct Tuple<> {};

	template<size_t N, class T>
	struct ElementType;

	template<class Head, class... Tail>
	struct Tuple<Head, Tail...>		// 这里是通过组合实现的，也可以通过继承实现 strut Tuple<Head, Tail...> : public Tuple<Tail...>
	{
		typedef Head HeadType;
		typedef Tuple<Tail...> TailType;

		Tuple(Head h, Tail... t)
			: tail(t...)
			, head(h)
		{
		}
		Tuple(Tuple<Head, Tail...> const& t)
			: tail(t.tail)
			, head(t.head)
		{
		}
		Tuple(Tuple<Head, Tail...>&& t)
			: tail(std::move(t.tail))
			, head(std::move(t.head))
		{
		}

		TailType tail;
		HeadType head;
		
	};

	//获取元组指定元素的值  
	template <size_t k>
	struct get_class 
	{
		template <class Head, class... Tail>
		static typename ElementType<k, Tuple<Head, Tail...>>::type& get(Tuple<Head, Tail...>& t)
		{
			return get_class<k - 1>::get(t.tail);
		}

		template <class Head, class... Tail>
		static typename ElementType<k, Tuple<Head, Tail...>>::type const& get(Tuple<Head, Tail...> const& t)
		{
			return get_class<k - 1>::get(t.tail);
		}
	};

	template <>
	struct get_class<0> 
	{
		template <class Head, class... Tail>
		static typename Tuple<Head, Tail...>::HeadType& get(Tuple<Head, Tail...>& t)
		{
			return t.head;
		}

		template <class Head, class... Tail>
		static typename Tuple<Head, Tail...>::HeadType const& get(Tuple<Head, Tail...> const& t)
		{
			return t.head;
		}
	};

	//推导元组指定元素的类型  
	template<size_t N, class T>
	struct ElementType
	{
	private:
		typedef typename T::TailType Next;
	public:
		typedef typename ElementType<N - 1, Next>::type type;
	};


	template<class T>
	struct ElementType<0, T>
	{
		typedef typename T::HeadType type;
	};


	template <size_t k, class Head, class... Tail>
	typename ElementType<k, Tuple<Head, Tail...>>::type& get(Tuple<Head, Tail...>& t)
	{
		return get_class<k>::get(t);
	}

	template <size_t k, class Head, class... Tail>
	typename ElementType<k, Tuple<Head, Tail...>>::type const& get(Tuple<Head, Tail...> const& t)
	{
		return get_class<k>::get(t);
	}

	template<class... Args>
	Tuple<Args...> Make_Tuple(Args&&... args)
	{
		return Tuple<Args...>(std::forward<Args>(args)...);
	}

}
