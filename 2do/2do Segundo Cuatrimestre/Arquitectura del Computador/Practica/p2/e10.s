.global main
main:
    pushq %rbp
    movq %rsp, %rbp

#a
    #movq $0x0123456789abcdef, %rax
    #rorq $32, %rax
#b
    movq $64, %rcx
    xorq %rdx, %rdx
    movq $0xaaaaaaaaaaaaaaaa, %rax
loop_count_1s:
    rorq $1, %rax
    adcq $0, %rdx
    loop loop_count_1s

    leaveq
    ret
