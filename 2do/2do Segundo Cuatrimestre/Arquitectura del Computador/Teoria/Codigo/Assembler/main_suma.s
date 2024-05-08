.data
y: .quad 25
x: .quad 45
str: .asciz "%d\n"


.text
.global main
main:
    movq y, %rdi
    movq x, %rsi
    movq $0, %rax
    call suma
    movq $str, %rdi
    movq %rax, %rsi
    movq $0, %rax
    call printf
    movq $0, %rax
    ret