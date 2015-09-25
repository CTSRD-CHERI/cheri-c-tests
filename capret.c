
void foo(void );

#ifdef __mips__
__asm__(
	".text\n"
	".globl	foo\n"
	".align	3\n"
	".type	foo,@function\n"
	".ent	foo\n"
"foo:                                    # @foo\n"
	".frame	$fp,32,$ra\n"
	".mask 	0x00000000,0\n"
	".fmask	0x00000000,0\n"
	"cjr	$c17\n"
	"nop\n"
	".end	foo\n"
"$func_end0:\n"
	".size	foo, ($func_end0)-foo\n"
);
#else
#error This test checks that return addresses are capabilities.
#endif

int main(void)
{
	foo();
	return 0;
}
