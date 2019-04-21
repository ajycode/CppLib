#pragma once

namespace Ajy
{

	namespace Traits
	{

		template<class T>
		using decay_t = typename std::decay<T>::type;

		//template<typename T> std::false_type static check_tuple_size(...);
		//template<typename T> std::true_type static  check_tuple_size(decltype(std::tuple_size<T>::value)*);
		template <template<class...> class U, class T>
		struct is_instantiation_of : std::false_type {};

		template <template <class...> class U, class... Args>
		struct is_instantiation_of< U, U<Args...> > : std::true_type {};

		template<class T>
		struct is_tuple : is_instantiation_of<std::tuple, T>
		{
		};

		//has_begin_end
		template<class T>
		struct has_begin_end
		{
		private:
			template<class U> static auto Check(int) -> decltype(std::declval<U>().begin(), std::declval<U>().end(), std::true_type());
			template<class U> static std::false_type Check(...);

		public:
			enum
			{
				value = std::is_same<decltype(Check<T>(0)), std::true_type>::value
			};
		};

		template <class T>
		struct has_const_iterator
		{
		private:
			template<class C> static std::true_type Check(typename C::const_iterator*);
			template<class C> static std::false_type  Check(...);
		public:
			enum
			{
				value = std::is_same<decltype(Check<T>(0)), std::true_type>::value
			};
		};

		template <class T>
		struct has_mapped_type
		{
		private:
			template<class C> static std::true_type Check(typename C::mapped_type*);
			template<class C> static std::false_type  Check(...);
		public:
			enum
			{
				value = std::is_same<decltype(Check<T>(0)), std::true_type>::value
			};
		};

		//#define HAS_XXX_TYPE(token)
		//	template <class T>struct has_##token{
		//	private:
		//	template<class C> static std::true_type Check(typename C::token##*); 
		//		template<class C> static std::false_type  Check(...);
		//	public:
		//		enum
		//		{
		//			value = std::is_same<decltype(Check<T>(0)), std::true_type>::value
		//		};
		//	};
		//
		//	HAS_XXX_TYPE(const_iterator)
		//	HAS_XXX_TYPE(mapped_type)

		template<class T> struct is_poiner_extent : std::false_type {};
		template<class T> struct is_poiner_extent<std::shared_ptr<T>> : std::true_type {};
		template<class T> struct is_poiner_extent<std::unique_ptr<T>> : std::true_type {};
		template<class T> struct is_poiner_extent<std::weak_ptr<T>> : std::true_type {};

		//#define IS_SMART_POINTER(token)
		//	template<class T> struct is_poiner_extent<std::token##_ptr<T>> : std::true_type{}; 
		//
		//	IS_SMART_POINTER(shared)
		//	IS_SMART_POINTER(unique)
		//	IS_SMART_POINTER(weak)

	}

	template <class Arary>
	struct is_std_array : std::false_type {};

	template <class T, std::size_t N>
	struct is_std_array<std::array<T, N>> : std::true_type {};

	template<class T>
	struct is_string : std::integral_constant<bool, std::is_same<Traits::decay_t<T>, std::string>::value> {};

	template <class T>
	struct is_container : public std::integral_constant<bool, Traits::has_const_iterator<Traits::decay_t<T>>::value&&Traits::has_begin_end<Traits::decay_t<T>>::value&&!is_string<T>::value> {};

	template <class T>
	struct is_singlevalue_container : public std::integral_constant<bool, !is_std_array<T>::value&&!std::is_array<Traits::decay_t<T>>::value&&!Traits::is_tuple<Traits::decay_t<T>>::value && is_container<Traits::decay_t<T>>::value&&!Traits::has_mapped_type<Traits::decay_t<T>>::value> {};

	template <class T>
	struct is_map_container : public std::integral_constant<bool, is_container<Traits::decay_t<T>>::value&&Traits::has_mapped_type<Traits::decay_t<T>>::value> {};

	template<class T>
	struct is_normal_class : std::integral_constant<bool, std::is_class<Traits::decay_t<T>>::value&&!is_string<T>::value>
	{};

	template<class T>
	struct is_basic_type : std::integral_constant<bool, std::is_arithmetic<Traits::decay_t<T>>::value || is_string<T>::value>
	{};

	template<class T>
	struct is_smart_pointer : Traits::is_poiner_extent<Traits::decay_t<T>> {};

	template<class T>
	struct is_pointer_ext : std::integral_constant<bool, std::is_pointer<Traits::decay_t<T>>::value || is_smart_pointer<Traits::decay_t<T>>::value> {};

	template <class T, template <class...> class Template>
	struct is_specialization_of : std::false_type {};

	template <template <class...> class Template, class... Args>
	struct is_specialization_of<Template<Args...>, Template> : std::true_type {};

	template<class T> struct is_tuple : is_specialization_of<Traits::decay_t<T>, std::tuple> {};
	template<class T> struct is_queue : is_specialization_of<Traits::decay_t<T>, std::queue> {};
	template<class T> struct is_stack : is_specialization_of<Traits::decay_t<T>, std::stack> {};
	template<class T> struct is_set : is_specialization_of<Traits::decay_t<T>, std::set> {};
	template<class T> struct is_multiset : is_specialization_of<Traits::decay_t<T>, std::multiset> {};
	template<class T> struct is_unordered_set : is_specialization_of<Traits::decay_t<T>, std::unordered_set> {};
	template<class T> struct is_priority_queue : is_specialization_of<Traits::decay_t<T>, std::priority_queue> {};
	template<class T> struct is_pair : is_specialization_of<Traits::decay_t<T>, std::pair> {};
	template<class T> struct is_cahash : is_specialization_of<Traits::decay_t<T>, Ajy::cache_affinity_hash> {};		
	//template<class T> struct is_cahash_map : is_specialization_of<Traits::decay_t<T>, Ajy::cahash_map> {};		// 错误判断方式，因为cahash_map是通过using弄出来的间接模板，所以这种方式判断有误
	template<class T> struct is_cahash_map : std::integral_constant<bool, is_cahash<T>::value && Traits::has_mapped_type<Traits::decay_t<T>>::value> {};	// 正确判断是否cahash_map的方式


	//#define IS_TEMPLATE_CLASS(token)
	//template<class T> struct is_##token : is_specialization_of<Traits::decay_t<T>, std::token##>{}; 
	//
	//IS_TEMPLATE_CLASS(tuple)
	//IS_TEMPLATE_CLASS(queue)
	//IS_TEMPLATE_CLASS(stack)
	//IS_TEMPLATE_CLASS(priority_queue)

	template<class T>
	struct is_container_adapter : std::integral_constant<bool, is_queue<T>::value || is_priority_queue<T>::value>
	{
	};

	template<class T>
	struct is_user_class : std::integral_constant<bool, is_normal_class<T>::value&&!is_container_adapter<T>::value&&!is_stack<T>::value&&!is_container<T>::value&&!is_tuple<T>::value&&!is_pair<T>::value>
	{};

	namespace Function_Traits
	{
		//普通函数.
		//函数指针.
		//function/lambda.
		//成员函数.
		//函数对象.

		//转换为std::function和函数指针. 
		template<class T>
		struct function_traits;

		//普通函数.
		template<class Ret, class... Params>
		struct function_traits<Ret(Params...)>
		{
		public:
			enum { paramCount = sizeof...(Params) };
			typedef Ret function_type(Params...);
			typedef Ret return_type;
			using stl_function_type = std::function<function_type>;
			typedef Ret(*pointer)(Params...);

			template<size_t I>
			struct args
			{
				static_assert(I < paramCount, "index is out of range, index must less than sizeof Args");
				using type = typename std::tuple_element<I, std::tuple<Params...>>::type;
			};
		};

		//函数指针.
		template<class Ret, class... Args>
		struct function_traits<Ret(*)(Args...)> : function_traits<Ret(Args...)> {};

		//std::function.
		template <class Ret, class... Args>
		struct function_traits<std::function<Ret(Args...)>> : function_traits<Ret(Args...)> {};

		//member function.
#define FUNCTION_TRAITS(...)\
template <class ReturnType, class ClassType, class... Args>\
struct function_traits<ReturnType(ClassType::*)(Args...) __VA_ARGS__> : function_traits<ReturnType(Args...)>{};\

		FUNCTION_TRAITS()
		FUNCTION_TRAITS(const)
		FUNCTION_TRAITS(volatile)
		FUNCTION_TRAITS(const volatile)

		//函数对象.
		template<class Function>
		struct function_traits : function_traits<decltype(&Function::operator())> {};

		template <class Function>
		typename function_traits<Function>::stl_function_type to_function(Function const& lambda)
		{
			return static_cast<typename function_traits<Function>::stl_function_type>(lambda);
		}

		template <class Function>
		typename function_traits<Function>::stl_function_type to_function(Function&& lambda)
		{
			return static_cast<typename function_traits<Function>::stl_function_type>(std::forward<Function>(lambda));
		}

		template <class Function>
		typename function_traits<Function>::pointer to_function_pointer(Function const& lambda)
		{
			return static_cast<typename function_traits<Function>::pointer>(lambda);
		}
	}

}
