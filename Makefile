SDK_ROOT ?= ~/sdk/
TESTS=\
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
	smallint\
	stack_cap\
	string\
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

test_runtime.o: test_runtime.c
	${SDK_ROOT}/bin/clang -c ${TEST_CFLAGS} $< -o $@

run.sh: run.sh.in
	sed 's/{INCLUDE_TESTS}/${TESTS}/g' run.sh.in > run.sh

clean:
	rm -f ${TESTS} test_runtime.o run.sh


%: %.c
