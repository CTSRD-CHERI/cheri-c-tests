#include "cheri_c_test.h"

typedef union {
	void *ptr;
	__uintcap_t intptr;
} PtrIntPair;

unsigned get_int(PtrIntPair p)
{
	return p.intptr & 7;
}

PtrIntPair set_int(PtrIntPair p, int val)
{
	val &= 7;
	p.intptr &= ~7LL;
	p.intptr ^= val;
	return p;
}

void *get_pointer(PtrIntPair p)
{
	return (void*)(p.intptr & ~7LL);
}

BEGIN_TEST
	char str[] = "123456789";
	PtrIntPair p;
	p.ptr = str;
	p = set_int(p, 4);
	assert(get_int(p) == 4);
	char *ptr = get_pointer(p);
	assert(ptr == str);
	assert(ptr[0] == '1');
END_TEST
