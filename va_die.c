#include <stdarg.h>
#include "cheri_c_test.h"

static volatile int faulted;
static void handler(void *capreg, int cause)
{
	faulted = 1;
}

void printstuff(char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	for (int i=1 ; i<10 ; i++)
	{
		int x = va_arg(ap, int);
		if (faulted)
		{
			assert(i == 9);
			break;
		}
		assert(x == i);
	}
	va_end(ap);
}

BEGIN_TEST
	test_fault_handler = handler;
	printstuff("%d\n", 1,2,3,4,5,6,7,8);
END_TEST
