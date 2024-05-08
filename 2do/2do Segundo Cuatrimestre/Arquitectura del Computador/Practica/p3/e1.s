.text
.global convert
convert:
    pushq %rbp
    movq %rsp, %rbp

    movq $0x3ff8000000000000, %rax
    movq %rax, -8(%rsp)
    movsd -8(%rsp), %xmm1
    mulsd %xmm1, %xmm0
    cvtsi2sdq %rdi, %xmm1
    addsd %xmm1, %xmm0

    leaveq
    ret
