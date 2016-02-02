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

volatile __intcap_t tmp;
volatile __intcap_t one = 1;

BEGIN_TEST(intcap)
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
	assert(one == 1);
END_TEST
