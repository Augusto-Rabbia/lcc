.data
    list: .long 10, 20, 30, 183, 40, 50, 125, 60, 70, 3, 80, 90, 100
    length: .byte 13
.text
.global sumlist
sumlist:
    pushq %rbp
    movq %rsp, %rbp

    xorq %rcx, %rcx
    movb %sil, %cl
    xorq %rax, %rax
loop_sumlist:
    addl -4(%rdi, %rcx, 4), %eax
    loop loop_sumlist

    leaveq
    ret

.global average
average:
    pushq %rbp
    movq %rsp, %rbp

    pushq %rsi
    call sumlist
    popq %rsi

    xorq %rdx, %rdx
    idivq %rsi

    leaveq
    ret

.global max
max:
    pushq %rbp
    movq %rsp, %rbp

    xorq %rcx, %rcx
    movb %sil, %cl
    movl -4(%rdi, %rcx, 4), %eax
    decq %rcx
loop_max:
    movl -4(%rdi, %rcx, 4), %edx
    cmpl %edx, %eax
    jge cont_max 
    movl %edx, %eax
cont_max:
    loop loop_max

    leaveq
    ret

.global min
min:
    pushq %rbp
    movq %rsp, %rbp

    xorq %rcx, %rcx
    movb %sil, %cl
    movl -4(%rdi, %rcx, 4), %eax
    decq %rcx
loop_min:
    movl -4(%rdi, %rcx, 4), %edx
    cmpl %edx, %eax
    jle cont_min 
    movl %edx, %eax
cont_min:
    loop loop_min

    leaveq
    ret

.global main
main:
    pushq %rbp
    movq %rsp, %rbp

    movq $list, %rdi
    xorq %rsi, %rsi
    movb length, %sil
    call min

    leaveq
    ret
