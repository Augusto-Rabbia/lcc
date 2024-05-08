.data
    format: .asciz "La cantidad de argumentos es %d. La suma es %d\n"
    format2: .asciz "No se ingresaron argumentos.\n"

.text
.global sumcuads
sumcuads:
    pushq %rbp
    movq %rsp, %rbp

    pushq %rbx
    pushq %rdi
    movq %rdi, %rcx
    xorq %rbx, %rbx
loop_argv:
    movq (%rsi, %rcx, 8), %rdi
    pushq %rcx
    pushq %rsi
    call atoi
    popq %rsi
    popq %rcx
    imulq %rax, %rax
    addq %rax, %rbx
    loop loop_argv

    movq %rbx, %rax
    popq %rdi
    popq %rbx
    leaveq
    ret

.global main
main:
    pushq %rbp
    movq %rsp, %rbp

    cmp $1, %rdi
    jz exit_error
    decq %rdi
    call sumcuads

    movq %rdi, %rsi
    movq $format, %rdi
    movq %rax, %rdx
    xorq %rax, %rax
    call printf

    leaveq
    ret

exit_error:
    movq $format2, %rdi
    xorq %rax, %rax
    call printf
    leaveq
    ret
