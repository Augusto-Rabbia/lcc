.global main
main:
    movq $31, %rax
    movq $5, %rbx
    movq $0, %rdx
    idiv %rbx
    ret


