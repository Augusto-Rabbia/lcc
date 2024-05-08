	.file	"Ej7.c"
	.text
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"ouch!"
	.text
	.p2align 4
	.globl	handler
	.type	handler, @function
handler:
.LFB61:
	.cfi_startproc
	endbr64
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	leaq	.LC0(%rip), %rdi
	call	puts@PLT
	movl	$1, denom(%rip)
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE61:
	.size	handler, .-handler
	.section	.rodata.str1.1
.LC1:
	.string	"r = %d\n"
	.section	.text.startup,"ax",@progbits
	.p2align 4
	.globl	main
	.type	main, @function
main:
.LFB62:
	.cfi_startproc
	endbr64
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	leaq	handler(%rip), %rsi
	movl	$8, %edi
	call	signal@PLT
	movl	$1, %eax
	movl	$1, %edi
	leaq	.LC1(%rip), %rsi
	cltd
	idivl	denom(%rip)
	movl	%eax, %edx
	xorl	%eax, %eax
	call	__printf_chk@PLT
	xorl	%eax, %eax
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE62:
	.size	main, .-main
	.globl	denom
	.bss
	.align 4
	.type	denom, @object
	.size	denom, 4
denom:
	.zero	4
	.ident	"GCC: (Ubuntu 9.4.0-1ubuntu1~20.04.1) 9.4.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	 1f - 0f
	.long	 4f - 1f
	.long	 5
0:
	.string	 "GNU"
1:
	.align 8
	.long	 0xc0000002
	.long	 3f - 2f
2:
	.long	 0x3
3:
	.align 8
4:
