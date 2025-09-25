	.file	"Homework3.c"
	.text
	.globl	foo
	.type	foo, @function
foo:
.LFB23:
	.cfi_startproc
	endbr64
	movl	$0, %ecx
	movl	$0, %eax
	jmp	.L2
.L3:
	sarl	%edi
	sarl	%esi
	addl	$1, %eax
.L2:
	cmpl	$31, %eax
	jg	.L5
	movl	%edi, %edx
	xorl	%esi, %edx
	testb	$1, %dl
	jne	.L3
	addl	$1, %ecx
	jmp	.L3
.L5:
	movl	%ecx, %eax
	ret
	.cfi_endproc
.LFE23:
	.size	foo, .-foo
	.ident	"GCC: (Ubuntu 13.3.0-6ubuntu2~24.04) 13.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
