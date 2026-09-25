#include "common.h"

enum
{
	CPS_UNKNOWN,
	CPS_RED_EARTH, // red earth
	CPS_SFIII, // sf3: new generation
	CPS_SFIII_2, // sf3: 2nd impact
	CPS_JOJO, // jojo's venture
	CPS_SFIII_3, // third strike
	CPS_JOJOBA, // hftf
	CPS__COUNT,
};

static const char* CPS_NAMES[] = {
	NULL,
	"redearth",
	"sfiii",
	"sfiii2",
	"jojo",
	"sfiii3",
	"jojoba",
};

static struct
{
	int source;
	const char* name;
} CPS_CLONES[] = {
	{CPS_JOJOBA,"jojobanr1"},
	{0,0},
};