.SUFFIXES:
.SUFFIXES: .c .o .dump .s .ll

SDK_ROOT ?= ~/sdk/
TESTS=\
	array\
	atomic\
	badcall\
	capcmp\
	capret\
	capretaddr\
	init\
	input\
	intcap\
	malloc\
	memcpy\
	memmove\
	null\
	output\
	printf\
	qsort\
	setjmp\
	smallint\
	stack_cap\
	string\
	union\
	va_args\
	va_copy\
	va_die

TEST_CFLAGS=-mabi=sandbox -cheri-linker -Werror -O3
TEST_LDFLAGS=-cheri-linker -lc -lmalloc_simple


all: $(TESTS) run.sh

install: all
	cp ${TESTS} run.sh ${DESTDIR}/

%: %.c test_runtime.o Makefile
	${SDK_ROOT}/bin/clang test_runtime.o ${TEST_CFLAGS} ${TEST_LDFLAGS} $< -o $@

%.ll: %.c Makefile
	${SDK_ROOT}/bin/clang ${TEST_CFLAGS} -S $< -o $@ -emit-llvm

%.s: %.c Makefile
	${SDK_ROOT}/bin/clang ${TEST_CFLAGS} -S $< -o $@

%.dump: %
	${SDK_ROOT}/bin/llvm-objdump -triple cheri-unknown-freebsd -d $< > $@

test_runtime.o: test_runtime.c
	${SDK_ROOT}/bin/clang -c ${TEST_CFLAGS} $< -o $@

run.sh: run.sh.in
	sed 's/{INCLUDE_TESTS}/${TESTS}/g' run.sh.in > run.sh

clean:
	rm -f ${TESTS} test_runtime.o run.sh


%: %.c
