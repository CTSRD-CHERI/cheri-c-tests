#include "cheri_c_test.h"

char foo[] = "01234";
// Check for initialisation of global pointers
volatile char *bar = (volatile char*)&foo;
typedef void(*fn)(void);

int called;
void test_fn_ptr(void)
{
	called++;
}
// Check that function pointers are correctly initialised both inside
// structures and bare:
struct f
{
	int x;
	fn f;
} x = {0, test_fn_ptr};

fn f = test_fn_ptr;

BEGIN_TEST
	// Check that the initialisation of a pointer to a global worked:
	assert((void*)bar == &foo);
	assert(bar[0] == '0');
	assert(bar[1] == '1');
	assert(bar[2] == '2');
	assert(bar[3] == '3');
	assert(bar[4] == '4');
	assert(bar[5] == 0);
	// Pointers to globals should not be executable capabilities
	assert((__builtin_memcap_perms_get((void*)bar) & __CHERI_CAP_PERMISSION_PERMIT_EXECUTE__) == 0);
	// At least for small allocations, this pointer should be exactly the size
	// of the global.
	assert(__builtin_memcap_length_get((void*)bar) == sizeof(foo));
	// Check that the two function pointers point to the correct place.
	assert(f == test_fn_ptr);
	assert(x.f == test_fn_ptr);
	// Pointers to functions should be executable capabilities
	assert((__builtin_memcap_perms_get(f) & __CHERI_CAP_PERMISSION_PERMIT_EXECUTE__) == __CHERI_CAP_PERMISSION_PERMIT_EXECUTE__);
	void *pcc = __builtin_memcap_program_counter_get();
	// Pointers to functions should be pcc with the offset set to the address
	// of the function.
	assert(__builtin_memcap_length_get(pcc) == __builtin_memcap_length_get(f));
	assert(__builtin_memcap_base_get(pcc) == __builtin_memcap_base_get(f));
	// That's all good in theory - now check that we can actually call the
	// functions!
	x.f();
	f();
	assert(called == 2);
END_TEST
