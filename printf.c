#include <stdio.h>
#include "cheri_c_test.h"

BEGIN_TEST
	char expected[] = "hello world";
	char out[sizeof(expected)+8];
	int len = sprintf(out, "hello %s", "world");
	assert(len == sizeof(expected)-1);
	for (int i=0 ; i<sizeof(expected) ; i++)
	{
		assert(expected[i] == out[i]);
	}
END_TEST
