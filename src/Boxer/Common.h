#pragma once

#include <cstdlib>
#include <cstring>
#include <cstdio>
#ifdef _MSC_VER
#include <intrin.h>
#endif
#include <immintrin.h>

typedef signed char			I8;
typedef signed short		I16;
typedef signed int			I32;
typedef signed long long	I64;

typedef unsigned char		U8;
typedef unsigned short		U16;
typedef unsigned int		U32;
typedef unsigned long long	U64;

typedef float				F32;
typedef double				F64;

typedef void* 				Handle;

// This is a cool feature that helps dealing with errors
// Its a Defer() macro. Essentially it calls a function when
// the scope is was created in is exitted.
template <typename F>
struct DeferInternal {
	F f;
	DeferInternal(F f) : f(f) {}
	~DeferInternal() { f(); }
};
template <typename F>
DeferInternal<F> defer_func(F f) {
	return DeferInternal<F>(f);
}
#define DEFER_1(x, y) x##y
#define DEFER_2(x, y) DEFER_1(x, y)
#define DEFER_3(x)    DEFER_2(x, __COUNTER__)
#define Defer(code)   auto DEFER_3(_defer_) = defer_func([&](){code;})

// Just basic assertion code
#define ASSERT(condition, ...) if (!(condition)) { printf("Assertion failed: " #condition ". " __VA_ARGS__); printf("\n"); }

#ifdef _DEBUG
#define DEBUG_ASSERT(condition, ...) ASSERT(condition, ...)
#else
#define DEBUG_ASSERT(condition, ...) 
#endif

#ifdef _MSC_VER
#define INLINE __forceinline
#else
// IDK
#define INLINE
#endif
