.global main
main:
    pushq %rbp
    movq %rbp, %rsp
//a
    movl $1, %eax
    sal $31, %eax
//b
    movl $1, %eax
    movl $1, %ebx
    sal $31, %eax
    sal $15, %ebx
    orl %ebx, %eax
//c
    movl $-1, %eax
    movl $0xffffff00, %ebx
    andl %ebx, %eax
//d
    movl $5, %eax
    sal $8, %eax
//e
    movl $-1, %eax
    movl $1, %ebx
    sal $8, %ebx
    notl %ebx
    andl %ebx, %eax

    leaveq
    retq
