#include <string.h>
#include "cheri_c_test.h"

BEGIN_TEST
	char str[] = "123456789";
	char str2[] = "123456789";
	char str3[sizeof(str2)];
	// For short strings, we should have byte-granularity lengths.
	// The other tests are checking that we don't overflow the end in string test.
	assert(__builtin_memcap_length_get(str) == sizeof(str));
	assert(strcmp(str, str2) == 0);
	assert(strlen(str) == 9);
	strcpy(str3, str2);
	assert(strcmp(str, str3) == 0);
	memcpy(str3, str2, sizeof(str2));
	assert(strcmp(str, str3) == 0);
END_TEST
