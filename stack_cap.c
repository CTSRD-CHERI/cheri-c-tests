#include "cheri_c_test.h"

const unsigned int sizes[] = {
	131072, 262144, 1048576
};
volatile void* ptrs[3];

void check_overlap(void *a, void*b)
{
	unsigned long long basea = __builtin_memcap_base_get(a);
	unsigned long long baseb = __builtin_memcap_base_get(b);
	unsigned long long topa = basea + __builtin_memcap_length_get(a);
	unsigned long long topb = baseb + __builtin_memcap_length_get(b);
	assert((basea >= topb) || (baseb >= topa));
}

void check_sizes(void)
{
	for (unsigned int i=0 ; i<sizeof(sizes)/sizeof(sizes[0]) ; i++)
	{
		assert(__builtin_memcap_length_get((void*)ptrs[i]) >= sizes[i]);
	}
}

BEGIN_TEST
	char foo[sizes[0]], bar[sizes[1]], baz[sizes[2]];
	ptrs[0] = foo;
	ptrs[1] = bar;
	ptrs[2] = baz;
	// Check that, even with alignment padding, none of the stack allocations overlap
	check_overlap(foo, bar);
	check_overlap(bar, baz);
	check_overlap(foo, baz);
	// Check that we have as much space as we asked for.
	check_sizes();
END_TEST

