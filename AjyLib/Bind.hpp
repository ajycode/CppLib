#pragma once

namespace Ajy
{

	namespace BindDetail
	{

		template<int...>
		struct sequence_t {};

		template<int I, int... sequence>
		struct make_sequence : make_sequence<I - 1, I - 1, sequence...> {};

		template<int... sequence>
		struct make_sequence<0, sequence...>
		{
			typedef sequence_t<sequence...> type;
		};

		template <int I>
		struct placeholder_t
		{
		};

		template<class T>
		struct is_placeholder;
		template<int I>
		struct is_placeholder<placeholder_t<I>>
		{
			enum
			{
				value = I
			};
		};
		template<class T>
		struct is_placeholder
		{
			enum
			{
				value = 0
			};
		};

		const placeholder_t<1> _1;
		const placeholder_t<2> _2;
		const placeholder_t<3> _3;
		const placeholder_t<4> _4;
		const placeholder_t<5> _5;
		const placeholder_t<6> _6;
		const placeholder_t<7> _7;
		const placeholder_t<8> _8;
		const placeholder_t<9> _9;
		const placeholder_t<10> _10;

		//// result type traits
		//template <class Function>
		//struct functionReturn_traits : result_traits<decltype(&Function::operator())> {};

		//template <class T>
		//struct functionReturn_traits<T*> : result_traits<T> {};

		///* check function */

		//template <class ReturnType, class... Params>
		//struct functionReturn_traits<ReturnType(*)(Params...)> { typedef ReturnType type; };

		///* check member function */
		//template <class ReturnType, class Class, class... Params>
		//struct functionReturn_traits<ReturnType(Class::*)(Params...)> { typedef ReturnType type; };

		template <class T, class Tuple>
		inline auto select(T&& val, Tuple&)->T&&
		{
			return std::forward<T>(val);
		}

		template <int I, class Tuple>
		inline auto select(placeholder_t<I>&, Tuple& tp) -> decltype(std::get<I - 1>(tp))
		{
			return std::get<I - 1>(tp);
		}

		template<class Fun, class... Params>
		struct Bind_t
		{
			typedef typename std::decay<Fun>::type bind_funtype_t;
			typedef std::tuple<typename std::decay<Params>::type...> bind_params_t;

			//typedef typename functionReturn_traits<bind_funtype_t>::type     fun_return_type;
			typedef typename Function_Traits::function_traits<bind_funtype_t>::return_type fun_return_type;
		public:
			template<class F, class... Args>
			Bind_t(F& f, Args&... args) : bind_fun(f), bind_params(args...)
			{
			}

			template<class F, class... Args>
			Bind_t(F&& f, Args&&... args) : bind_fun(std::move(f)), bind_params(std::move(args)...)
			{}

			template <class... Args>
			fun_return_type operator()(Args&&... args)
			{
				return call(make_sequence<std::tuple_size<bind_params_t>::value>::type(),std::forward_as_tuple(std::forward<Args>(args)...));
			}

		private:

			template<class ArgTuple, int... indexes>
			fun_return_type call(sequence_t<indexes...>& idxes, ArgTuple& argTp)
			{
				return InvokeDetail::invoke<fun_return_type>(bind_fun, select(std::get<indexes>(bind_params), argTp)...);
			}

		private:
			bind_funtype_t bind_fun;
			bind_params_t bind_params;
		};

	}

	template <class Fun, class... Params>
	inline BindDetail::Bind_t<Fun, Params...> Bind(Fun&& fun, Params&&... params)
	{
		return BindDetail::Bind_t<Fun, Params...>(std::forward<Fun>(fun), std::forward<Params>(params)...);
	}

	template <class Fun, class... Params>
	inline BindDetail::Bind_t<Fun, Params...> Bind(Fun& fun, Params&... params)
	{
		return BindDetail::Bind_t<Fun, Params...>(fun, params...);
	}

}

