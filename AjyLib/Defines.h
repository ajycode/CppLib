#pragma once

/*--------------------copy from others*/

/* CPU(X86) - i386 / x86 32-bit */
#if defined(__i386__) || defined(i386) || defined(_M_IX86) || defined(_X86_) || defined(__THW_INTEL)
#define Ajy_IA32
#endif

/* CPU(X86_64) - AMD64 / Intel64 / x86_64 64-bit */
#if defined(__x86_64__) || defined(_M_X64)
#define Ajy_IA64
#endif

/* CPU - INTEL / AMD */
#if defined(Ajy_IA32) || defined(Ajy_IA64)
#define Ajy_IA
#endif

/* Compile - X86 / X64 */
#if defined(__LP64__) || defined(_WIN64) || defined(_M_X64)
#define Ajy_X64
#else
#define Ajy_X86
#endif

#if defined(Ajy_IA64) || defined(Ajy_X64)
#define Ajy_64
#else
#define Ajy_32
#endif

/* Compile - DEBUG / RELEASE */
#if defined(NDEBUG) && !defined(DEBUG)
#define Ajy_RELEASE
#else
#define Ajy_DEBUG
#endif

/* CPU - big endian / little endian */
#if defined(BIGENDIAN) || defined(BIG_ENDIAN)
#define Ajy_BE
#else
#define Ajy_LE
#endif

/* OS - windows, ios, android, linux 这里完全不考虑 mac osx */
#ifdef _WIN32
#define Ajy_WINDOWS
#elif defined(__APPLE__)
#define Ajy_IOS
#elif defined(__ANDROID__)
#define Ajy_ANDROID
#endif
#ifdef __linux
#define Ajy_LINUX
#endif

/* Compiler - ARM RealView Compilation Tools */
#if defined(__CC_ARM) || defined(__ARMCC__)
#define Ajy_RVCT
#endif

/* Compiler - GNU Compiler Collection */
/* --gnu option of the RVCT compiler also defines __GNUC__ */
#if defined(__GNUC__) && !defined(Ajy_RVCT)
#define Ajy_GCC
#endif

/* Compiler - Microsoft Visual C++ */
#if defined(_MSC_VER)
#define Ajy_MSVC
#endif

/* Compile - enforce inline */
#if defined(Ajy_GCC) && defined(Ajy_RELEASE) && !defined(__MINGW32__)
#define Ajy_INLINE inline __attribute__((__always_inline__))
#elif (defined(Ajy_MSVC) || defined(Ajy_GCC)) && defined(Ajy_RELEASE)
#define Ajy_INLINE __forceinline
#else
#define Ajy_INLINE inline
#endif

/* Compile - enforce noinline */
#if defined(Ajy_GCC)
#define Ajy_NOINLINE __attribute__((__noinline__))
#elif defined(Ajy_MSVC) || defined(Ajy_RVCT)
#define Ajy_NOINLINE __declspec(noinline)
#else
#define Ajy_NOINLINE
#endif

/* CPU - branch prediction: unlikely */
#ifndef Ajy_UNLIKELY
#if defined(Ajy_GCC)
#define Ajy_UNLIKELY(x) __builtin_expect((x), 0)
#else
#define Ajy_UNLIKELY(x) (x)
#endif
#endif

/* CPU - branch prediction: likely */
#ifndef Ajy_LIKELY
#if defined(Ajy_GCC)
#define Ajy_LIKELY(x) __builtin_expect((x), 1)
#else
#define Ajy_LIKELY(x) (x)
#endif
#endif

/* Memory address align */
#if defined(Ajy_MSVC)
#define Ajy_ALIGN2( x )		    __declspec(align(2)) x
#define Ajy_ALIGN4( x )		    __declspec(align(4)) x
#define Ajy_ALIGN8( x )		    __declspec(align(8)) x
#define Ajy_ALIGN16( x )		    __declspec(align(16)) x
#define Ajy_ALIGN32( x )		    __declspec(align(32)) x
#elif defined(Ajy_GCC)
#define Ajy_ALIGN2( x )          x __attribute__ ((aligned (2)))
#define Ajy_ALIGN4( x )          x __attribute__ ((aligned (4)))
#define Ajy_ALIGN8( x )          x __attribute__ ((aligned (8)))
#define Ajy_ALIGN16( x )         x __attribute__ ((aligned (16)))
#define Ajy_ALIGN32( x )         x __attribute__ ((aligned (32)))
#else
#define Ajy_ALIGN2( x )	        x
#define Ajy_ALIGN4( x )	        x
#define Ajy_ALIGN8( x )	        x
#define Ajy_ALIGN16( x )         x
#define Ajy_ALIGN32( x )         x
#endif

/*--------------------copy from others*/






/* Common types for easy use */
typedef int8_t          int8, sbyte, Int8, SByte, Sbyte;
typedef int16_t         int16, Int16;
typedef int32_t         int32, Int32;
typedef int64_t         int64, Int64;
typedef uint8_t         uint8, Byte, UInt8, Uint8;
typedef uint16_t        uint16, ushort, UInt16, Uint16, UShort, Ushort;
typedef uint32_t        uint32, uint, UInt32, Uint32, Uint, UInt;
typedef uint64_t        uint64, UInt64, Uint64;
#ifdef Ajy_32
typedef int32_t         int3264, Int3264;
typedef uint32_t        uint3264, UInt3264, Uint3264;
#else
typedef int64_t         int3264, Int3264;
typedef uint64_t        uint3264, UInt3264, Uint3264;
#endif
/*
// 关于 size_t:
// vc 下的 size_t 等同于 unsigned long long, 没有 ssize_t
// gcc 下的 size_t 等同于 gcc 的 unsigned long, ssize_t = long
*/
#ifdef Ajy_MSVC
typedef int64_t         ssize_t;
#endif
// long 数据类型是不推荐使用的. gcc 下它的长度为 32 或 64bit,  vc 下它的长度为 32
typedef long            Long;
typedef unsigned long   ulong, ULong, Ulong;



namespace Ajy
{

	// inherit this class that mean memory can be safe memmove
	class MemoryMoveable
	{
	};

	class NonCopyable
	{
	public:
		NonCopyable(NonCopyable const&) = delete; // deleted
		NonCopyable& operator = (NonCopyable const&) = delete; // deleted
		NonCopyable() = default;   // available
	};

}
