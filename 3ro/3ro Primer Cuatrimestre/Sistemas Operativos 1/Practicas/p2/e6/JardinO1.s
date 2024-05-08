	.file	"e6.c"
	.text
	.globl	entran_visitantes
	.type	entran_visitantes, @function
entran_visitantes:
.LFB52:
	.cfi_startproc
	endbr64
	movl	visitantes(%rip), %edx
	leal	1(%rdx), %eax
	addl	$10001, %edx
.L2:
	movl	%eax, %ecx
	addl	$1, %eax
	cmpl	%edx, %eax
	jne	.L2
	movl	%ecx, visitantes(%rip)
	ret
	.cfi_endproc
.LFE52:
	.size	entran_visitantes, .-entran_visitantes
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"VISITANTES:%d\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB53:
	.cfi_startproc
	endbr64
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	subq	$32, %rsp
	.cfi_def_cfa_offset 48
	movq	%fs:40, %rax
	movq	%rax, 24(%rsp)
	xorl	%eax, %eax
	leaq	8(%rsp), %rdi
	movl	$0, %ecx
	leaq	entran_visitantes(%rip), %rbx
	movq	%rbx, %rdx
	movl	$0, %esi
	call	pthread_create@PLT
	leaq	16(%rsp), %rdi
	movl	$0, %ecx
	movq	%rbx, %rdx
	movl	$0, %esi
	call	pthread_create@PLT
	movl	$0, %esi
	movq	8(%rsp), %rdi
	call	pthread_join@PLT
	movl	$0, %esi
	movq	16(%rsp), %rdi
	call	pthread_join@PLT
	movl	visitantes(%rip), %edx
	leaq	.LC0(%rip), %rsi
	movl	$1, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
	movq	24(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L7
	movl	$0, %eax
	addq	$32, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 16
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
.L7:
	.cfi_restore_state
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE53:
	.size	main, .-main
	.globl	visitantes
	.bss
	.align 4
	.type	visitantes, @object
	.size	visitantes, 4
visitantes:
	.zero	4
	.ident	"GCC: (Ubuntu 11.3.0-1ubuntu1~22.04) 11.3.0"
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
