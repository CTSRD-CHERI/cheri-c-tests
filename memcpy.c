/*-
 * Copyright (c) 2012-2015 David Chisnall
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
#include <string.h>
#include "cheri_c_test.h"

// Test structure which will be memcpy'd.  Contains data and a capability in
// the middle.  The capability must be aligned, but memcpy should work for any
// partial copy of this structure that includes the capability, as long as both
// have the correct alignment.
struct Test 
{
	char pad0[32];
	void *y;
	char pad1[32];
};

// Check that the copy has the data that we expect it to contain.  The start
// and end parameters describe the range in the padding to check.  For partial
// copies, the uncopied range will contain nonsense.
void check(struct Test *t1, int start, int end)
{
	for (int i=start ; i<32 ; i++)
	{
		assert(t1->pad0[i] == i);
	}
	assert((void*)t1->y == t1);
	assert(__builtin_memcap_tag_get(t1->y));
	for (int i=0 ; i<end ; i++)
	{
		assert(t1->pad1[i] == i);
	}
}

// Write an obviously invalid byte pattern over the output structure.
void invalidate(struct Test *t1)
{
	unsigned char *x = (unsigned char*)t1;
	for (int i=0 ; i<sizeof(*t1) ; i++)
	{
		*x = 0xa5;
	}
}

BEGIN_TEST(memcpy, "memcpy")
	struct Test t1, t2;

	invalidate(&t2);
	for (int i=0 ; i<32 ; i++)
	{
		t1.pad0[i] = i;
		t1.pad1[i] = i;
	}
	t1.y = &t2;
	invalidate(&t2);
	// Simple case: aligned start and end
	__capability void *cpy = memcpy(t1.y, &t1, sizeof(t1));
	assert((void*)cpy == &t2);
	check(&t2, 0, 32);
	invalidate(&t2);
	// Test that it still works with an unaligned start...
	cpy = memcpy(&t2.pad0[3], &t1.pad0[3], sizeof(t1) - 3);
	assert((void*)cpy == &t2.pad0[3]);
	check(&t2, 3, 32);
	// ...or an unaligned end...
	cpy = memcpy(&t2, &t1, sizeof(t1) - 3);
	assert((void*)cpy == &t2);
	check(&t2, 0, 29);
	// ...or both...
	cpy = memcpy(&t2.pad0[3], &t1.pad0[3], sizeof(t1) - 6);
	assert((void*)cpy == &t2.pad0[3]);
	check(&t2, 3, 29);
	invalidate(&t2);
	// ...and finally a case where the alignment is different for both?
	cpy = memcpy(&t2, &t1.pad0[1], sizeof(t1) - 1);
	assert((void*)cpy == &t2);
	// This should have invalidated the capability
	assert(__builtin_memcap_tag_get(t2.y) == 0);
	// Check that the non-capability data has been copied correctly
	for (int i=0 ; i<31 ; i++)
	{
		assert(t2.pad0[i] == i+1);
		assert(t2.pad1[i] == i+1);
	}
	invalidate(&t2);
	// Simple case: aligned start and end
	void *copy = memcpy(&t2, &t1, sizeof(t1));
	assert(copy == &t2);
	check(&t2, 0, 32);
	invalidate(&t2);
	// Test that it still works with an unaligned start...
	copy = memcpy(&t2.pad0[3], &t1.pad0[3], sizeof(t1) - 3);
	assert(copy == &t2.pad0[3]);
	check(&t2, 3, 32);
	// ...or an unaligned end...
	copy = memcpy(&t2, &t1, sizeof(t1) - 3);
	assert(copy == &t2);
	check(&t2, 0, 29);
	// ...or both...
	copy = memcpy(&t2.pad0[3], &t1.pad0[3], sizeof(t1) - 6);
	assert(copy == &t2.pad0[3]);
	check(&t2, 3, 29);
	invalidate(&t2);
	// ...and finally a case where the alignment is different for both?
	copy = memcpy(&t2, &t1.pad0[1], sizeof(t1) - 1);
	assert(copy == &t2);
	// This should have invalidated the capability
	assert(!__builtin_memcap_tag_get(t2.y));
	// Check that the non-capability data has been copied correctly
	for (int i=0 ; i<31 ; i++)
	{
		assert(t2.pad0[i] == i+1);
		assert(t2.pad1[i] == i+1);
	}
	
	// .. and finally finally tests that offsets are taken into
	// account when checking alignment.  These are regression tests
	// for a bug in memcpy.

	// aligned base, unaligned offset + base
	invalidate(&t2);
	cpy = memcpy(
		__builtin_memcap_offset_increment(&t2, 3),
		__builtin_memcap_offset_increment(&t1, 3),
		sizeof(t1)-6
		);
	assert((void*)cpy == &t2.pad0[3]);
	check(&t2, 3, 29);
END_TEST

