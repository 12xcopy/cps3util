#pragma once
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <memory.h>


#if defined (_WIN32)
#define WINDOWS 1
#define POSIX 0
#else
#define WINDOWS 0
#define POSIX 1
#endif

#if defined(__x86_64__) || defined(_M_X64)
#define IS_X86 1
#else
#define IS_X86 0
#endif


#define Export __attribute__((visibility("default")))
#define Import extern
#define ForceInline __attribute__((always_inline)) inline


#include <stdbool.h>
#undef false
#undef true
enum
{
	false,
	true,
};
typedef bool bool_t;

#ifndef NULL
#define NULL ((void*)0)
#endif

#ifndef strdupa 
#define strdupa(STR) strcpy(alloca(strlen(STR) + 1), STR)
#define StrDupA i forbid this since its name is confusingly similar to strdupa. just use strdup
#endif

#define memdup(PTR,SZ) memcpy(malloc(SZ),PTR,SZ);

#define rtassert_abort() { printf("\nassertion failed.\n file:%s\n line:%i\n sr:%s\n",__FILE__,__LINE__,__PRETTY_FUNCTION__);abort(); }
#define rtassert(X) if(!(X)) { rtassert_abort(); }
#define rtassert_msgf(X,MSG,...) if(!(X)) { fprintf(stderr,MSG,__VA_ARGS__); rtassert_abort(); }
#define rtassert_msg(X,MSG) if(!(X)) { fputs(MSG,stderr); rtassert_abort(); }

typedef long long long_t; // i hate using long long
typedef unsigned long long ulong_t; 

static double floor(double x)
{
	bool negative = x < 0;
	if (negative) return ((int)(x  - 0.995));
	return (int)x;
}

static double fract(double x)
{
	return x - floor(x);
}