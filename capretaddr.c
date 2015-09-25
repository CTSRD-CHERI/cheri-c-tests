#include "cheri_c_test.h"

int main(void);
void foo(void)
{
	void *ret = __builtin_return_address(0);
	// Check that the return capability is, indeed, a capability
	assert(__builtin_memcap_tag_get(ret) == 1);
	// Return capability should be executable
	assert((__builtin_memcap_perms_get(ret) & __CHERI_CAP_PERMISSION_PERMIT_EXECUTE__) == __CHERI_CAP_PERMISSION_PERMIT_EXECUTE__);
	// Return capability offset should be after the pcc-relative offset of main.
	assert(__builtin_memcap_offset_get(ret) > __builtin_memcap_offset_get(main));
	// Approximate, but main really shouldn't need to be more than 100
	// instruction in any vaguely sane implementation.
	assert(__builtin_memcap_offset_get(ret) - __builtin_memcap_offset_get(main) < 100);
	// We shouldn't be able to write through code capabilities
	XFAIL((__builtin_memcap_perms_get(ret) & __CHERI_CAP_PERMISSION_PERMIT_STORE_CAPABILITY__) == 0);
	XFAIL((__builtin_memcap_perms_get(ret) & __CHERI_CAP_PERMISSION_PERMIT_STORE__) == 0);
}

int main(void)
{
	foo();
	return 0;
}
