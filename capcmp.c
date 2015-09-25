#include <string.h>
#include "cheri_c_test.h"

BEGIN_TEST
	char str[] = "123456789";
	char *end = &str[sizeof(str)];
	// Pointer to one-past-the-end should work
	assert(__builtin_memcap_offset_get(end) == __builtin_memcap_length_get(end));
	for (int i=1 ; i<sizeof(str) ; i++)
	{
		char *ptr = &str[i];
		// Derived capabilities should not change the bounds unless explicitly requested
		assert(__builtin_memcap_base_get(ptr) == __builtin_memcap_base_get(str));
		assert(__builtin_memcap_length_get(ptr) == __builtin_memcap_length_get(str));
		// Pointer comparison should work
		assert(ptr > str);
		assert(ptr < end);
	}
END_TEST
