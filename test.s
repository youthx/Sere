	.text
	.file	"<string>"
	.globl	__global__
	.p2align	4, 0x90
	.type	__global__,@function
__global__:
	retq
.Lfunc_end0:
	.size	__global__, .Lfunc_end0-__global__

	.globl	main
	.p2align	4, 0x90
	.type	main,@function
main:
	xorl	%eax, %eax
	retq
.Lfunc_end1:
	.size	main, .Lfunc_end1-main

	.type	strtmp_2,@object
	.section	.rodata,"a",@progbits
	.globl	strtmp_2
strtmp_2:
	.asciz	"Hello, world!"
	.size	strtmp_2, 14

	.section	".note.GNU-stack","",@progbits
