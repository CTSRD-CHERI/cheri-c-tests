#include <stdarg.h>
#include "cheri_c_test.h"

static volatile int faulted;
static void handler(void *capreg, int cause)
{
	faulted = 1;
}
char str[] = "012345678901234567890";
volatile void *ptrs[] = 
{
	&str[0],
	&str[1],
	&str[2],
	&str[3],
	&str[4],
	&str[5],
	&str[6],
	&str[7],
	&str[8],
	&str[9],
	&str[0],
	&str[11],
	&str[12],
	&str[13],
	&str[14],
	&str[15],
	&str[16],
	&str[17],
	&str[18],
	&str[19]
};

void printstuff(int argpairs, ...)
{
	va_list ap;
	va_start(ap, argpairs);
	// Check that the length corresponds to the number of arguments, with
	// appropriate padding.
	assert(__builtin_memcap_length_get(ap) == argpairs * sizeof(void*) * 2);
	for (int i=0 ; i<argpairs ; i++)
	{
		int x = va_arg(ap, int);
		char *p = va_arg(ap, void*);
		assert(x == i);
		assert(p == ptrs[i]);
		assert(*p = str[i]);
	}
	va_end(ap);
}

BEGIN_TEST
	test_fault_handler = handler;
	printstuff(8, 0,ptrs[0],1,ptrs[1],2,ptrs[2],3,ptrs[3],4,ptrs[4],5,ptrs[5],6,ptrs[6],7,ptrs[7]);
END_TEST
