#include "cheri_c_test.h"

volatile __intcap_t tmp;

BEGIN_TEST
	char str[] = "0123456789";
	__intcap_t foo = 42;
	assert(__builtin_memcap_tag_get((void*)foo) == 0);
	assert(__builtin_memcap_offset_get((void*)foo) == 42);
	assert(__builtin_memcap_base_get((void*)foo) == 0);
	foo = (__intcap_t)str;
	assert(__builtin_memcap_tag_get((void*)foo) == 1);
	assert(__builtin_memcap_tag_get((void*)foo) == 1);
	foo += 5;
	assert((*(char*)foo) == '5');
	assert(__builtin_memcap_offset_get((void*)foo) == 5);
	assert(__builtin_memcap_base_get((void*)foo) == __builtin_memcap_base_get(str));
	assert(__builtin_memcap_length_get((void*)foo) == __builtin_memcap_length_get(str));
	foo += 50;
	// Ensure that the +50 is not removed
	tmp = foo;
	foo = tmp;
	foo -= 50;
	assert((*(char*)foo) == '5');
	assert(__builtin_memcap_offset_get((void*)foo) == 5);
	// Valid capabilities are strictly ordered after invalid ones
	assert(0xffffffffffffffffULL < foo);
END_TEST
