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
	NULL,
};

static struct
{
	int source;
	const char* name;
} CPS_CLONES[] = {
	{CPS_JOJOBA,"jojobanr1"},
	{0,0},
};



// adapted from mame's source code
// https://github.com/mamedev/mame/blob/master/src/mame/capcom/cps3.cpp

#if 0
static uint16_t rotate_left(uint16_t value, int n)
{
	
	int aux = value>>(16-n);
	return ((value<<n)|aux)%0x10000;
}
#else
#include <x86intrin.h>
static ForceInline uint16_t rotate_left(uint16_t value, int n)
{
	return __rolw(value,n);
}
static ForceInline uint16_t rotate_right(uint16_t value, int n)
{
	return __rorw(value,n);
}
#endif

static uint16_t rotxor(uint16_t val, uint16_t xorval)
{
	uint16_t res = val + rotate_left(val,2);
	res = rotate_left(res,4) ^ (res & (val ^ xorval));
	return res;
}
static uint32_t cps3_mask(uint32_t address, uint32_t key1, uint32_t key2)
{
	if (key1 == 0)
		return 0;

	address ^= key1;
	uint16_t val = (address & 0xffff) ^ 0xffff;
	val = rotxor(val, key2 & 0xffff);
	val ^= (address >> 16) ^ 0xffff;
	val = rotxor(val, key2 >> 16);
	val ^= (address & 0xffff) ^ (key2 & 0xffff);
	return val | (val << 16);
}


static struct {
	long_t a,b;
} CPS_KEYS[] = {
	{0,0},
	{0x9e300ab1,0xa175b82c},
	{0xb5fe053e,0xfc03925a}, // ng
	{0,0}, // 2nd impact
	{0x02203ee3,0x01301972}, // jojo
	{0xa55432b4,0x0c129981}, // 3rd
	{0x23323ee3,0x03021972}, // heritage

};