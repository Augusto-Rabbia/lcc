.data
    cadenalarga: .asciz "123456"
    cadenacorta: .asciz "123" #=>1 , 124=>0, 456=>0
.text
.global cmp_str
cmp_str:
    #retorna 1 si las cadenas son iguales en los primeros %edx caracteres
    #retorna 0 si son distintas
    pushq %rbp
    movq %rsp, %rbp
    
    movl %edx, %ecx
    xorq %rdx, %rdx
loop_cmp_str:
    #movb (%rdi, %rdx), %r8b
    lodsb
    #cmp %r8b, %al
    cmp (%rdi, %rdx), %al
    jne diferentes_cmp_str
    incq %rdx
    loop loop_cmp_str

    movq $1, %rax
    leaveq
    ret
diferentes_cmp_str:
    movq $0, %rax
    leaveq
    ret

.global main
main:
    movq $cadenalarga, %rdi
    movq $cadenacorta, %rsi
    movl $3,%edx #debe ser la longitud de cadenacorta
    call cmp_str
    ret
