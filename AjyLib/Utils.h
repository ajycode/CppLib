#pragma once

#ifdef Ajy_GCC
#include <kernel.h>
#else
#ifndef offsetof   
#define offsetof(type, member) ((size_t) &((type *)0)->member)   
#endif
#define container_of(ptr,type,member) ((type *)((char *)ptr-offsetof(type,member)))
#endif // Ajy_GCC

namespace Ajy
{

#ifndef MIN
#define MIN( a, b )  ( (a) < (b) ? (a) : (b) )
#endif
#ifndef MAX
#define MAX( a, b )  ( (a) > (b) ? (a) : (b) )
#endif

	// for C++11 ( same as  C++14  std::make_index_sequence )
	template<size_t... T>
	struct IndexSequence
	{
		typedef IndexSequence<T...> type;
	};

	template<size_t... T>
	struct MakeIndexSequence;

	template<>
	struct MakeIndexSequence < 0 >
	{
		typedef IndexSequence<>::type type;
	};

	template<>
	struct MakeIndexSequence < 1 >
	{
		typedef IndexSequence<0>::type type;
	};

	template<size_t... TN>
	struct MakeIndexSequence < 0, TN... >
	{
		typedef typename IndexSequence<0, TN...>::type type;
	};

	template<size_t N>
	struct MakeIndexSequence < N >
	{
		typedef typename MakeIndexSequence<N - 2, N - 1>::type type;
	};

	template<size_t N, size_t... TN>
	struct MakeIndexSequence < N, TN... >
	{
		typedef typename MakeIndexSequence<N - 1, N, TN...>::type type;
	};



	template<class PairParam>
	struct Get1st
	{
		typename PairParam::first_type const& operator()(PairParam const& p) const { return p.first; }
	};

	template<class Param>
	struct GetWhole
	{
		Param const& operator()(Param const& x) const { return x; }
	};

	template<class T, class... Args>
	class MaxSizeOf
	{
	public:
		enum
		{
			value = sizeof(T) > MaxSizeOf<Args...>::value
			? sizeof(T)
			: MaxSizeOf<Args...>::value
		};
	};
	template<class T>
	class MaxSizeOf<T>
	{
	public:
		enum
		{
			value = sizeof(T)
		};
	};
	template<class T, class... Args>
	class MaxAlignOf
	{
	public:
		enum
		{
			value = std::alignment_of<T>::value > MaxAlignOf<Args...>::value
			? std::alignment_of<T>::value
			: MaxAlignOf<Args...>::value
		};
	};
	template<class T>
	class MaxAlignOf<T>
	{
	public:
		enum
		{
			value = std::alignment_of<T>::value
		};
	};
	template<class T, class... Args>
	struct MaxType : std::integral_constant<int,(sizeof(T)>MaxType<Args...>::value ? sizeof(T) : MaxType<Args...>::value)>
	{};
	template<class T>
	struct MaxType<T> : std::integral_constant<int, sizeof(T) > {};

	/** 是否包含某个类型 */
	template<class T, class... Types>
	struct TypeContains;
	template<class T, class Head, class... Tail>
	struct TypeContains<T, Head, Tail...>
		: std::conditional<std::is_same<T, Head>::value, std::true_type, TypeContains<T, Tail... >>::type
	{
	};
	template<class T>
	struct TypeContains<T> : std::false_type
	{
	};

	template<class T, class... Types>
	struct IndexOfTypes;
	template<class T, class Head, class... Tail>
	struct IndexOfTypes<T, Head, Tail...>
	{
		enum { value = IndexOfTypes<T, Tail...>::value + 1 };
	};
	template<class T, class... Types>
	struct IndexOfTypes<T, T, Types...>
	{
		enum { value = 0 };
	};
	template<class T>
	struct IndexOfTypes<T>
	{
		enum { value = -1 };
	};

	template<int index, int maxSize>
	struct CheckTypeIndexValid
	{
		static_assert(index >= 0, "Index is out of range!");
		static_assert(index < maxSize, "Index is out of range!");
	};
	template<int index, class... Types>
	struct TypesAt;
	template<int index, class Head, class... Tail>
	struct TypesAt<index, Head, Tail...>
	{
		using type = typename TypesAt<index - 1, Tail...>::type;
	};
	template<class T, class... Types>
	struct TypesAt<0, T, Types...>
	{
		using type = T;
	};


	template<class... Args>
	class HasParms
	{
	public:
		static bool const value = sizeof...(Args) > 0;
	};

	bool IsPrime(size_t candidate);
	// 得到刚好小于 n 的质数 主用于内存分配
	size_t GetPrime(size_t n, bool isLess = true);
	//// 返回一个 内存占用为 2^N ( capacity * dataSize ) 的 大于等于原值的 质数 capacity
	//int32 GetPrime(size_t capacity, size_t dataSize);

	// calc lead zero （数 2 进制数高位头上的 0 的个数
	int32 Clz(size_t x);
	int32 Popcnt(size_t x);


	// 返回刚好大于 x 的 2^n 的值用于内存分配
	// 如果 x 本身 2^n 系 则返回原值
	size_t Round2n(size_t len);

	// 算一个数是 2 的几次方
	size_t Calc2n(size_t len);


	/************************************************************************/
	/* 计算哈希值                                                            */
	/************************************************************************/

	size_t GetHashCode_CS(uint8 const* buf, size_t len);             // if arm, the buf must be align of 4(32bit)/8(64bit)
	size_t GetHashCode_Lua(uint8 const* buf, size_t len);            // if len <=4, will be faster than CS
	size_t GetHashCode(std::string const& in);

	template<class T>
	size_t GetHashCode(T const& in);
	template<class T>
	size_t GetHashCode(T*		in);
	size_t GetHashCode(int8		in);
	size_t GetHashCode(uint8	in);
	size_t GetHashCode(int16	in);
	size_t GetHashCode(uint16	in);
	size_t GetHashCode(int32	in);
	size_t GetHashCode(uint32	in);
	size_t GetHashCode(uint64	in);
	size_t GetHashCode(int64	in);
	size_t GetHashCode(float	in);
	size_t GetHashCode(double	in);
	size_t GetHashCode(void*	in);

	template<class T>
	bool Equals(T const& a, T const& b);
	template<class T>
	bool Equals(T* a, T* b);
	template<class T>
	void Swap(T& left, T& right);


	/************************************************************************/
	/* 字符串相关                                                            */
	/************************************************************************/

	// 取长度系列，值为 ToString 后的最大长. 10 进制
	class String;
	template<class T>
	size_t GetToStringMaxLength(T t)
	{
		return 1;
	}
	size_t GetToStringMaxLength(uint8       v);
	size_t GetToStringMaxLength(uint16      v);
	size_t GetToStringMaxLength(uint32      v);
	size_t GetToStringMaxLength(uint64      v);   // vc's size_t = uint64
	size_t GetToStringMaxLength(int8        v);
	size_t GetToStringMaxLength(int16       v);
	size_t GetToStringMaxLength(int32       v);
	size_t GetToStringMaxLength(int64       v);   // vc havn't ssize_t, so typedef it
	size_t GetToStringMaxLength(double      v);
	size_t GetToStringMaxLength(float       v);
	size_t GetToStringMaxLength(bool        v);
	size_t GetToStringMaxLength(char        v);
	size_t GetToStringMaxLength(long        v);   // gcc's ssize_t = long
	size_t GetToStringMaxLength(ulong       v);   // gcc's size_t = ulong
	size_t GetToStringMaxLength(char const* v);
	size_t GetToStringMaxLength(std::string const& v);
	size_t GetToStringMaxLength(String const& v);

	// 将值类型转换为字符串 返回转换后的串长 (主要为 Append, Write 等函数服务)
	size_t ToString(char* dstBuf, uint8       v);
	size_t ToString(char* dstBuf, uint16      v);
	size_t ToString(char* dstBuf, uint32      v);
	size_t ToString(char* dstBuf, uint64      v);
	size_t ToString(char* dstBuf, int8        v);
	size_t ToString(char* dstBuf, int16       v);
	size_t ToString(char* dstBuf, int32       v);
	size_t ToString(char* dstBuf, int64       v);
	size_t ToString(char* dstBuf, double      v);
	size_t ToString(char* dstBuf, float       v);
	size_t ToString(char* dstBuf, bool        v);
	size_t ToString(char* dstBuf, char        v);
	size_t ToString(char* dstBuf, long        v);
	size_t ToString(char* dstBuf, ulong       v);
	size_t ToString(char* dstBuf, char const* v);
	size_t ToString(char* dstBuf, std::string const& v);
	size_t ToString(char* dstBuf, String const& v);

	// HEX 版
	size_t ToHexString(char* dstBuf, uint8  v);
	size_t ToHexString(char* dstBuf, uint16 v);
	size_t ToHexString(char* dstBuf, uint32 v);
	size_t ToHexString(char* dstBuf, uint64 v);
	size_t ToHexString(char* dstBuf, int8   v);
	size_t ToHexString(char* dstBuf, int16  v);
	size_t ToHexString(char* dstBuf, int32  v);
	size_t ToHexString(char* dstBuf, int64  v);
	size_t ToHexString(char* dstBuf, char   v);
	size_t ToHexString(char* dstBuf, long   v);
	size_t ToHexString(char* dstBuf, ulong  v);

	// char* 转为各种长度的 有符号整数. Out 取值范围： int8~64
	template <class OutType>
	void toInt(char const* in, OutType& out);
	// char* (不能有减号打头) 转为各种长度的 无符号整数. Out 取值范围： uint8, uint16, uint32, uint64
	template <class OutType>
	void toUInt(char const* in, OutType& out);
	void FromString(uint8& out, char const* in);
	void FromString(uint16& out, char const* in);
	void FromString(uint32& out, char const* in);
	void FromString(uint64& out, char const* in);
	void FromString(int8& out, char const* in);
	void FromString(int16& out, char const* in);
	void FromString(int32& out, char const* in);
	void FromString(int64& out, char const* in);
	void FromString(double& out, char const* in);
	void FromString(float& out, char const* in);
	void FromString(bool& out, char const* in);
	void FromString(long& out, char const* in);
	void FromString(ulong& out, char const* in);


	/************************************************************************/
	/* Hasher                                                               */
	/************************************************************************/

	HAS_MEMBER_FUNCTION(HasFunc_GetHashCode, GetHashCode, size_t(T::*)() const)
	
	template<class T>
	struct Hasher
	{
		template<class U, class = typename std::enable_if<std::is_convertible<U, T>::value, void>::type>
		typename std::enable_if<HasFunc_GetHashCode<U>::value, size_t>::type operator()(U const& in) const
		{
			return in.GetHashCode();
		}

		template<class U, class = typename std::enable_if<std::is_convertible<U, T>::value, void>::type>
		typename std::enable_if<!HasFunc_GetHashCode<U>::value, size_t>::type operator()(U const& in) const
		{
			return GetHashCode(in);
		}
	};
	template<>
	struct Hasher<int8>
	{
		size_t operator()(int8 const& in) const
		{
			return (size_t)in;
		}
	};
	template<>
	struct Hasher<uint8>
	{
		size_t operator()(uint8 const& in) const
		{
			return (size_t)in;
		}
	};
	template<>
	struct Hasher<int16>
	{
		size_t operator()(int16 const& in) const
		{
			return (size_t)in;
		}
	};
	template<>
	struct Hasher<uint16>
	{
		size_t operator()(uint16 const& in) const
		{
			return (size_t)in;
		}
	};
	template<>
	struct Hasher<int32>
	{
		size_t operator()(int32 const& in) const
		{
			return (size_t)in;
		}
	};
	template<>
	struct Hasher<uint32>
	{
		size_t operator()(uint32 const& in) const
		{
			return (size_t)in;
		}
	};
	template<>
	struct Hasher<int64>
	{
		size_t operator()(int64 const& in) const
		{
			return (size_t)(((uint64)in & 0xFFFFFFFFu) ^ ((uint64)in >> 32));
		}
	};
	template<>
	struct Hasher<uint64>
	{
		size_t operator()(uint64 const& in) const
		{
			return (size_t)((in & 0xFFFFFFFFu) ^ (in >> 32));
		}
	};
	template<>
	struct Hasher<float>
	{
		size_t operator()(float const& in) const
		{
			return *(size_t*)(float*)&in;
		}
	};
	template<>
	struct Hasher<double>
	{
		size_t operator()(double const& in) const
		{
			uint64 val = *(uint64*)(double*)&in;
			return (size_t)((val & 0xFFFFFFFFu) ^ (val >> 32));
		}
	};

	
}

