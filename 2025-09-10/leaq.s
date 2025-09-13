	.file	"leaq.c"
	.text
	.globl	scale
	.type	scale, @function
scale:
.LFB0:
	.cfi_startproc
	endbr64
	leaq	(%rdi,%rsi,4), %rax  	# %rax = x + 4y
	leaq	(%rdx,%rdx,2), %rdx  	# %rdx = z + 2z = 3z
	leaq	(%rax,%rdx,4), %rax  	# %rax = (x + 4y) + 4(3z) = x + 4y + 12z
	ret
	.cfi_endproc
.LFE0:
	.size	scale, .-scale
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
