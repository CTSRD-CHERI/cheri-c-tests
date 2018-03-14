/*-
 * Copyright (c) 2012 David T. Chisnall
 * All rights reserved.
 *
 * This software was developed by SRI International and the University of
 * Cambridge Computer Laboratory under DARPA/AFRL contract FA8750-10-C-0237
 * ("CTSRD"), as part of the DARPA CRASH research programme.
 *
 * @BERI_LICENSE_HEADER_START@
 *
 * Licensed to BERI Open Systems C.I.C. (BERI) under one or more contributor
 * license agreements.  See the NOTICE file distributed with this work for
 * additional information regarding copyright ownership.  BERI licenses this
 * file to you under the BERI Hardware-Software License, Version 1.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at:
 *
 *   http://www.beri-open-systems.org/legal/license-1-0.txt
 *
 * Unless required by applicable law or agreed to in writing, Work distributed
 * under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations under the License.
 *
 * @BERI_LICENSE_HEADER_END@
 */

#if !__has_feature(cheri_casts)
#warning "Compiler doesn't support cheri_casts, please update!"
#define __cheri_cast
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define TO_CAP(x) ((__cheri_tocap void * __capability)(void*)(x))

typedef __attribute__((memory_address)) long vaddr_t;


__attribute__((noreturn)) int __assert_fail(int);
inline void  __assert(int cond, int line)
{
	if (!cond)
	{
		__assert_fail(line);
	}
}
#define assert(cond) __assert(cond, __LINE__)

void __assert_eq_long(int line, long actual, long expected);
#define assert_eq(actual, expected) __assert_eq_long(__LINE__, actual, expected)

void __assert_eq_cap(int line, void* __capability actual, void* __capability expected);
#define assert_eq_cap(actual, expected) __assert_eq_cap(__LINE__, actual, expected)


// Dumps a value into a specified register.  Useful for debugging test cases.
#define DEBUG_DUMP_REG(regno, val) \
    __asm__ volatile ("dadd $" #regno ", %0, $0" : : "r" (val) : #regno);

// Add a nop
#define DEBUG_NOP() \
	__asm__ volatile ("nop")


extern volatile long long exception_count;

#define success_if_no_exceptions() __extension__({assert_eq(exception_count, 0); 0;})
#ifdef __cplusplus
} // end of exern "C"
#endif
