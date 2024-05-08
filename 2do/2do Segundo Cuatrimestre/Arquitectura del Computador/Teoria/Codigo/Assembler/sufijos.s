.data
a: .quad 0x1122334455667788
b: .long  0x11223344
c: .word 0x1122
d: .byte 0x11
f: .float 3.14
g: .double 3.14

.text
.global main
main:

        movq a, %rax
        movl b, %eax
        movw c, %ax
        movb d, %al
        movss f, %xmm0
        movsd g, %xmm0
        ret
