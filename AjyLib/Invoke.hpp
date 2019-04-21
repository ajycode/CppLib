#pragma once

namespace Ajy
{

	namespace InvokeDetail
	{

		// The invoker for call a callable
		template <class T>
		struct is_pointer_noref
			: std::is_pointer<typename std::remove_reference<T>::type>
		{};

		template <class T>
		struct is_memfunc_noref
			: std::is_member_function_pointer<typename std::remove_reference<T>::type>
		{};

		template <class ReturnType, class Fun, class... Params>
		inline typename std::enable_if<is_pointer_noref<Fun>::value, ReturnType>::type invoke(Fun&& fun, Params&&... params)
		{
			return (*std::forward<Fun>(fun))(std::forward<Params>(params)...);
		}

		template <class ReturnType, class Fun, class... Params>
		inline typename std::enable_if<!is_pointer_noref<Fun>::value && !is_memfunc_noref<Fun>::value, ReturnType>::type invoke(Fun&& fun, Params&&... params)
		{
			return std::forward<Fun>(fun)(std::forward<Params>(params)...);
		}

		template <class ReturnType, class Fun, class Class, class... Params>
		inline typename std::enable_if<is_memfunc_noref<Fun>::value && is_pointer_noref<Class>::value, ReturnType>::type invoke(Fun&& fun, Class&& classPtr, Params&&... params)
		{
			return (std::forward<Class>(classPtr)->*std::forward<Fun>(fun))(std::forward<Params>(params)...);
		}

		template <class ReturnType, class Fun, class Class, class... Params>
		inline typename std::enable_if<is_memfunc_noref<Fun>::value && !is_pointer_noref<Class>::value, ReturnType>::type invoke(Fun&& fun, Class&& classObj, Params&&... params)
		{
			return (std::forward<Class>(classObj).*std::forward<Fun>(fun))(std::forward<Params>(params)...);
		}

	}

}

