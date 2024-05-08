.global sum
sum:
    pushq %rbp
    movq %rsp, %rbp

    movl %edx, %ecx
sum_loop:
    movss -4(%rsi, %rcx, 4), %xmm0
    addss -4(%rdi, %rcx, 4), %xmm0
    movss %xmm0, -4(%rdi, %rcx, 4)
    loop sum_loop

    leaveq
    ret
