/*-
 * Copyright (c) 2015 David Chisnall
 * All rights reserved.
 *
 * This software was developed by SRI International and the University of
 * Cambridge Computer Laboratory under DARPA/AFRL contract (FA8750-10-C-0237)
 * ("CTSRD"), as part of the DARPA CRASH research programme.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
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

BEGIN_TEST(init)
	// Check that the initialisation of a pointer to a global worked:
	assert(bar[0] == '0');
	assert(bar[1] == '1');
	assert(bar[2] == '2');
	assert(bar[3] == '3');
	assert(bar[4] == '4');
	assert(bar[5] == 0);
	// Pointers to globals should not be executable capabilities
	ASSERT_HAS_NOT_PERMISSION(bar, EXECUTE);
	// At least for small allocations, this pointer should be exactly the size
	// of the global.
	assert(__builtin_memcap_length_get((void*)bar) == sizeof(foo));
	assert((void*)bar == &foo);
	// Check that the two function pointers point to the correct place.
	assert(f == test_fn_ptr);
	assert(x.f == test_fn_ptr);
	// Pointers to functions should be executable capabilities
	ASSERT_HAS_PERMISSION(f, EXECUTE);
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
