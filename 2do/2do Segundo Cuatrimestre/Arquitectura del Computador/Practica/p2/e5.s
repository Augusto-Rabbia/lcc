.global main
main:
    pushq %rbp
    movq %rsp, %rbp

    movl $-1, %eax
    movl $2, %ecx
    imull %ecx



    xorq %rax, %rax


    leaveq
    retq
