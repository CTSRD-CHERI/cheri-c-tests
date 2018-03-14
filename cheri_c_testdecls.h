/*-
 * Copyright (c) 2016 SRI International
 * All rights reserved.
 *
 * This software was developed by SRI International and the University of
 * Cambridge Computer Laboratory under DARPA/AFRL contract FA8750-10-C-0237
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

/*
 * Each test should be declared one per line with a DECLARE_TEST for
 * ordinary tests and a DECLARE_TEST_FAULT test for ones that require a
 * fault handler.
 *
 * The name of each test must be the name of the .c file implementing it.
 *
 * No ifndef guard to allow multiple includes with different definitions
 * of DECLARE_TEST and DECLARE_TEST_FAULT.
 */

DECLARE_TEST_FAULT(array, "on-stack array")
DECLARE_TEST(atomic, "C11 atomic types")
DECLARE_TEST_FAULT(badcall, "calling non-functions")
DECLARE_TEST(capcmp, "pointer comparision")
DECLARE_TEST(capret, "return addresses are capabilities")
DECLARE_TEST(capretaddr, "return address")
DECLARE_TEST(funptr, "function pointers")
DECLARE_TEST(init, "global pointer initalization")
DECLARE_TEST_FAULT(input, "__input qualifier")
DECLARE_TEST(intcap, "__intcap_t init, math, comparison")
DECLARE_TEST(malloc, "malloc")
DECLARE_TEST(memcpy, "memcpy")
DECLARE_TEST(memmove, "memmove")
DECLARE_TEST(null, "generate NULL pointers")
DECLARE_TEST_FAULT(output, "__output qualifier")
DECLARE_TEST(printf, "printf")
DECLARE_TEST(qsort, "qsort")
DECLARE_TEST(setjmp, "setjmp, longjmp")
DECLARE_TEST(smallint, "small integer math on pointers")
DECLARE_TEST(stack_cap, "os-stack arrays")
DECLARE_TEST(string, "on stack strings")
DECLARE_TEST(int64math, "math on int64_t")
DECLARE_TEST(uint64math, "math on uint64_t")
DECLARE_TEST(intcapmath, "math on __intcap_t")
DECLARE_TEST(uintcapmath, "math on __uintcap_t")
DECLARE_TEST(union, "unions with capabilities")
DECLARE_TEST(byval_args, "by value function parameters")
DECLARE_TEST_FAULT(va_args, "varidac functions")
DECLARE_TEST(va_copy, "va_copy copies")
DECLARE_TEST_FAULT(va_die, "consume too many varargs")

DECLARE_TEST(clang_bcopy, "memcpy");
DECLARE_TEST(clang_cast, "capability to array");
DECLARE_TEST(clang_cursor, "capability offset arithmetic");
DECLARE_TEST_FAULT(clang_cursor_trivial, "capability offset faults");
DECLARE_TEST(clang_load_data, "load data via capability");
DECLARE_TEST(clang_load_double, "load doubles via capability");
DECLARE_TEST(clang_load_float, "load floats via capability");
/* XXX-LPT: expect to unexpectedly fault due to DDC missing CHERI_PERM_SEAL */
DECLARE_TEST(clang_opaque, "seal/unseal capability");
DECLARE_TEST(clang_pack, "unaligned store");
DECLARE_TEST(clang_store_data, "store data via capability");
DECLARE_TEST(clang_struct, "capability to static struct");
DECLARE_TEST(clang_sub, "subtract capabilities");
DECLARE_TEST(clang_toy, "summate return");
