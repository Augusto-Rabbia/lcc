.data
    cero: .float 0.0

.text
.global solve
solve:
    pushq %rbp
    movq %rsp, %rbp

    movss %xmm3, %xmm6      #xmm6 = d
    divss %xmm0, %xmm6      #xmm6 = d/a
    movss %xmm6, %xmm7      #xmm7 = d/a
    mulss %xmm1, %xmm6      #xmm6 = (d/a)*b
    subss %xmm6, %xmm4      #xmm4 = (xmm4 = e) - (d/a)*b

    ucomiss cero, %xmm4
    jz singular

    mulss %xmm2, %xmm7      #xmm7 = (d/a)*c
    subss %xmm7, %xmm5      #xmm5 = (xmm5 = f) - (d/a)*c
    divss %xmm5, %xmm4      #xmm4 = xmm4/xmm5 = y
    movss %xmm5, (%rsi)
    mulss %xmm5, %xmm1      #
    subss %xmm1, %xmm2
    divss %xmm0, %xmm2
    movss %xmm2, (%rdi)

    xorq %rax, %rax
    jmp salida

singular:
    movq $-1, %rax
salida:
    leaveq
    ret
