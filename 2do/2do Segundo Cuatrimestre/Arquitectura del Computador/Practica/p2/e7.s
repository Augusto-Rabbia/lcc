.text
.global fact1
fact1:
    pushq %rbp
    movq %rsp, %rbp

    cmpq $1, %rdi
    jz exit_fact1
    imulq %rdi, %rax
    leaveq
    ret

exit_fact1:
    movq %rdi, %rax
    leaveq
    ret

.global fact2
fact2:
    pushq %rbp
    movq %rsp, %rbp

    movq $1, %rax
    movq %rdi, %rcx
loop_fact2:
    imulq %rcx
    loop loop_fact2

    leaveq
    ret

.global main
main:
    pushq %rbp
    movq %rsp, %rbp

    movq $5, %rdi
    call fact2

    leaveq
    ret
