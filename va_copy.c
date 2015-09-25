#include <stdio.h>
#include <stdarg.h>

void printstuff(char *fmt, ...)
{
	va_list ap, ap2;
	va_start(ap, fmt);
	va_copy(ap2, ap);
	while (1) {
		int x = va_arg(ap, int);
		printf(fmt, x);
		x = va_arg(ap2, int);
		printf(fmt, x);
	}
	va_end(ap);
}

int main(void)
{
	printstuff("%d\n", 1,2,3,4,5,6,7,8);
	//printf("hello %s", "world");
	return 0;
}
