SDK_ROOT ?= ~/sdk/
TESTS=\
	  capcmp\
	capret\
	capretaddr\
	init\
	intcap\
	printf\
	smallint\
	stack_cap\
	string\
	va_args\
	va_copy\
	va_die

TEST_CFLAGS=-mabi=sandbox -cheri-linker -Werror
TEST_LDFLAGS=-cheri-linker -lc -lmalloc_simple


all: $(TESTS)

install: all
	cp ${TESTS} ${DESTDIR}/

%: %.c test_runtime.o Makefile
	${SDK_ROOT}/bin/clang test_runtime.o ${TEST_CFLAGS} ${TEST_LDFLAGS} $< -o $@

test_runtime.o: test_runtime.c
	${SDK_ROOT}/bin/clang -c ${TEST_CFLAGS} $< -o $@

clean:
	rm -f ${TESTS} test_runtime.o

%: %.c
