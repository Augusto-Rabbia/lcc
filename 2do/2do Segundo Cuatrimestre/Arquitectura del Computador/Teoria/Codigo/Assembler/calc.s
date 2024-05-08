.global calc
calc:
	pushq %rbp
	movq %rsp, %rbp
	addq %rdi, %rsi
	movq %rsi, %rax
	movq %rbp, %rsp
	popq %rbp
	ret
