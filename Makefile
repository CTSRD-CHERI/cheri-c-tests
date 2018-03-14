.SUFFIXES:
.SUFFIXES: .c .o .dump .s .ll

SDK_ROOT ?= ~/sdk/

TESTS=$(TESTS_CLANG_PURECAP) $(TESTS_CLANG_HYBRID) $(TESTS_LIBC)
TESTS_CLANG_PURECAP_DIR=clang-purecap
TESTS_CLANG_HYBRID_DIR=clang-hybrid
TESTS_CLANG_PURECAP:=\
	array\
	atomic\
	badcall\
	capcmp\
	capret\
	capretaddr\
	funptr\
	init\
	input\
	int64math\
	intcap\
	null\
	output\
	smallint\
	stack_cap\
	uint64math\
	uintcapmath\
	union\
	va_args\
	va_copy\
	va_die
TESTS_CLANG_HYBRID:=\
	test_clang_cursor\
	test_clang_load_double\
	test_clang_pack\
	test_clang_sub\
	test_clang_bcopy\
	test_clang_cursor_trivial\
	test_clang_load_float\
	test_clang_store_data\
	test_clang_toy\
	test_clang_cast\
	test_clang_load_data\
	test_clang_opaque\
	test_clang_struct
TESTS_LIBC_DIR=libc
TESTS_LIBC:=\
	malloc\
	memcpy\
	memmove\
	printf\
	qsort\
	setjmp\
	string
TESTS_CLANG_PURECAP:=$(addprefix $(TESTS_CLANG_PURECAP_DIR)/, $(TESTS_CLANG_PURECAP))
TESTS_CLANG_HYBRID:=$(addprefix $(TESTS_CLANG_HYBRID_DIR)/, $(TESTS_CLANG_HYBRID))
TESTS_LIBC:=$(addprefix $(TESTS_LIBC_DIR)/, $(TESTS_LIBC))

CFLAGS=-mabi=purecap -msoft-float -cheri-linker -Werror -O3
CFLAGS+=-DHAVE_MALLOC_USUABLE_SIZE
CFLAGS+=-I.
CFLAGS_TESTS_CLANG_=$(CFLAGS:-mabi%=)
CFLAGS_TESTS_CLANG_HYBRID=-mabi=n64 $(CFLAGS_TESTS_CLANG_)
CFLAGS_TESTS_CLANG_PURECAP =-mabi=purecap $(CFLAGS_TESTS_CLANG_)

LDFLAGS=-cheri-linker -lc -lmalloc_simple
LDFLAGS_TESTS_CLANG_HYBRID=$(LDFLAGS:-lmalloc_simple=)


all: $(TESTS_CLANG_PURECAP) $(TESTS_CLANG_HYBRID) $(TESTS_LIBC) run.sh

install: all
	cp ${TESTS} run.sh ${DESTDIR}/

%: %.c test_runtime.o Makefile
	${SDK_ROOT}/bin/clang test_runtime.o ${CFLAGS} ${LDFLAGS} $< -o $@

$(TESTS_CLANG_PURECAP_DIR)/%: $(TESTS_CLANG_PURECAP_DIR)/%.c test_runtime.o Makefile
	${SDK_ROOT}/bin/clang test_runtime.o $(CFLAGS_TESTS_CLANG_PURECAP) $(LDFLAGS) $< -o $@

$(TESTS_CLANG_HYBRID_DIR)/%: $(TESTS_CLANG_HYBRID_DIR)/%.c $(TESTS_CLANG_HYBRID_DIR)/test_runtime.o Makefile
	${SDK_ROOT}/bin/clang $(TESTS_CLANG_HYBRID_DIR)/test_runtime.o $(CFLAGS_TESTS_CLANG_HYBRID) $(LDFLAGS_TESTS_CLANG_HYBRID) $< -o $@

%.ll: %.c Makefile
	${SDK_ROOT}/bin/clang ${CFLAGS} -S $< -o $@ -emit-llvm

%.s: %.c Makefile
	${SDK_ROOT}/bin/clang ${CFLAGS} -S $< -o $@

%.dump: %
	${SDK_ROOT}/bin/llvm-objdump -triple cheri-unknown-freebsd -d $< > $@

test_runtime.o: test_runtime.c
	${SDK_ROOT}/bin/clang -c ${CFLAGS} $< -o $@

$(TESTS_CLANG_HYBRID_DIR)/test_runtime.o: test_runtime.c
	${SDK_ROOT}/bin/clang -c ${CFLAGS_TESTS_CLANG_HYBRID} $< -o $@

run.sh: run.sh.in
	sed 's/{INCLUDE_TESTS}/${TESTS}/g' run.sh.in > run.sh

clean:
	rm -f ${TESTS} test_runtime.o $(TESTS_CLANG_HYBRID_DIR)/test_runtime.o run.sh


%: %.c

# inc64math.c, intcapmath.c, and uint64math.c include uintcapmath.c
int64math:	uintcapmath.c
intcapmath:	uintcapmath.c
uint64math:	uintcapmath.c
