#pragma once
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <memory.h>

#define Export __attribute__((visibility("default")))
#define Import extern


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

#define rtassert(X) if(!(X)) { abort(); }
#define rtassert_msgf(X,MSG,...) if(!(X)) { fprintf(stderr,MSG,__VA_ARGS__); abort(); }
#define rtassert_msg(X,MSG) if(!(X)) { fputs(MSG,stderr); abort(); }




typedef long long long_t; // i hate using long long
typedef unsigned long long ulong_t; 