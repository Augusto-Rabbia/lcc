.data
    format: .asciz "%08x\n"
    i: .quad 0xDEADBEEF
.text
.global main
main:
    pushq %rbp
    movq %rsp, %rbp

    movq $format, %rdi
    movq i, %rsi
    xorq %rax, %rax
    call printf

    xorq %rax, %rax
    leaveq
    ret
