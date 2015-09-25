#include <assert.h>
#include <stdint.h>

#ifndef __FreeBSD__
#warning The test suite is currently expected to work only on FreeBSD.
#endif

typedef void (*cheri_handler)(void *, int);

extern cheri_handler test_fault_handler;

void test_setup(void);

#define BEGIN_TEST \
	int main(void) { test_setup(); 
#define END_TEST return 0; }

#ifdef INCLUDE_XFAIL
#define XFAIL(x) assert(x)
#else
#define XFAIL(x) do {} while(0)
#endif

