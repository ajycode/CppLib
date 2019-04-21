#pragma once

namespace Ajy
{

	template<class BeCheckClass, bool is_const_memberfunc, class FuncRet, class ...FuncParams>
	class ConstMemberFunctionHelper
	{
	public:
		typedef FuncRet(BeCheckClass::*FuncSignature)(FuncParams...);
		//using FuncSignature = FuncRet(BeCheckClass::*)(FuncParams...);
	};

	template<class BeCheckClass, class FuncRet, class ...FuncParams>
	class ConstMemberFunctionHelper<BeCheckClass, true, FuncRet, FuncParams...>
	{
	public:
		typedef FuncRet(BeCheckClass::*FuncSignature)(FuncParams...) const;
		//using FuncSignature = FuncRet(BeCheckClass::*)(FuncParams...) const;
	};

	// 只支持public权限的非静态成员

	/************************************************************************/
	/* CN->HelperClassName                                                  */
	/* FN->FunctionName                                                     */
	/* CT->ClassTemplate                                                    */
	/* FT->FunctionTemplate                                                 */
	/************************************************************************/
#define HAS_MEMBER_FUNCTION( CN, FN, FT )													\
template<class CT>																			\
class CN																					\
{																							\
    template<class T, FT> class FuncMatcher;												\
    template<class T> static char HasMemberFunc( FuncMatcher<T, &T::FN>* );					\
    template<class T> static int HasMemberFunc( ... );										\
public:																						\
    static bool const value = sizeof( HasMemberFunc<CT>( nullptr ) ) == sizeof(char);		\
};

#define HAS_MEMBER_FUNCTION_V2( CheckHelperClassName, FuncName, Is_const_memberfunc, FuncRet, ...)			\
template<class BeCheckedClass>																			\
class CheckHelperClassName : ConstMemberFunctionHelper<BeCheckedClass, Is_const_memberfunc, FuncRet, __VA_ARGS__>	\
{																							\
    template<class TYPE, FuncSignature> class FuncMatcher;												\
    template<class TYPE> static char HasMemberFunc( FuncMatcher<TYPE, &TYPE::FuncName>* );					\
    template<class TYPE> static int HasMemberFunc( ... );										\
public:																						\
    static bool const value = sizeof(HasMemberFunc<BeCheckedClass>(nullptr)) == sizeof(char);		\
};

#define HAS_MEMBER_FUNCTION_V3(BeCheckFuncName)			\
template<class BeCheckClass, bool Is_Const_Memberfunc, class FuncRet, class ...FuncParams>																			\
class HAS_MEMBER_FUNCTION_##BeCheckFuncName : ConstMemberFunctionHelper<BeCheckClass, Is_Const_Memberfunc, FuncRet, FuncParams...>	\
{																							\
    template<class TYPE, FuncSignature> class FuncMatcher;												\
    template<class TYPE> static char HasMemberFunc( FuncMatcher<TYPE, &TYPE::BeCheckFuncName>* );					\
    template<class TYPE> static int HasMemberFunc( ... );										\
public:																						\
    static bool const value = sizeof(HasMemberFunc<BeCheckClass>(nullptr)) == sizeof(char);		\
};

//#define HAS_MEMBER_FUNCTION_V4(BeCheckedFuncName)

// 对于只是因CV符修饰不同而导致的重载行为的检测存在问题
#define HAS_FUNCTION_MEMBER(member)																\
template<class T, class... Args>														\
struct Has_MemberFunc_##member																\
{																						\
private:																				\
		template<class U> static auto Check(int) -> decltype(std::declval<U>().member(std::declval<Args>()...), std::true_type());	\
		template<class U> static std::false_type Check(...);							\
public:																					\
		static bool const value = std::is_same<decltype(Check<T>(0)), std::true_type>::value;	\
};


#define HAS_MEMBER_TYPE( CN, TYPE )															\
template<class CT>																			\
class CN																					\
{																							\
    template<class T> static char HasMemberType( typename T::TYPE* );						\
    template<class T> static int HasMemberType( ... );										\
public:																						\
    static bool const value = sizeof( HasMemberType<CT>( nullptr ) ) == sizeof(char);		\
};


	template<typename... Args> struct make_void { typedef void type; };
	template<typename... Args> using void_tt = typename make_void<Args...>::type;
#define HAS_TYPE_MEMBER(MEMBER)																\
    template<typename T, typename = void>													\
    struct has_type_##MEMBER : std::false_type												\
    {																						\
    };																						\
    template<typename T>																	\
    struct has_type_##MEMBER < T, void_tt<typename T::MEMBER>>: std::true_type				\
    {																						\
    };


	/*

	SFINAE check menber function exists
	sample：

	HAS_MEMBER_FUNCTION( FUNC_NAME_checker, T_MEMBER_FUNC_NAME, RETURN_TYPE ( T::* )( T_MEMBER_FUNC_PARMS ) const );

	template<class T>
	typename std::enable_if<FUNC_NAME_checker<T>::value, RETURN_TYPE>::type FUNC_NAME_switch( T const& v )
	{
	return v.T_MEMBER_FUNC_NAME( T_MEMBER_FUNC_PARMS );
	};
	template<class T>
	typename std::enable_if<!FUNC_NAME_checker<T>::value, RETURN_TYPE>::type FUNC_NAME_switch( T const& v )
	{
	return................;
	};

	template<class T>
	RETURN_TYPE FUNC_NAME( T const& v )
	{
	return FUNC_NAME_switch( v );
	}

	*/

}
