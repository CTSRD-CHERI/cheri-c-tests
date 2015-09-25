#include <stdarg.h>
#include "cheri_c_test.h"

void printstuff(char *fmt, ...)
{
	va_list ap, ap2;
	va_start(ap, fmt);
	va_copy(ap2, ap);
	assert(__builtin_memcap_length_get((void*)ap) == __builtin_memcap_length_get((void*)ap2));
	for (int i=1 ; i<9 ; i++)
	{
		int x = va_arg(ap, int);
		int y = va_arg(ap2, int);
		assert(y == y);
	}
	va_end(ap);
}

BEGIN_TEST
	printstuff("%d\n", 1,2,3,4,5,6,7,8);
END_TEST
