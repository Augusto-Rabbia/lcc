.global main
main:
    movb $-1, %al
    addb $1, %al
    movl $-1, %eax
    addl $1, %eax
    ret

