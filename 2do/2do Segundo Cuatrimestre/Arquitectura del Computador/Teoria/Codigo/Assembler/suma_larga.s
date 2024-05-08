.data
str: .string "Suma = %d\n"

.text
.global main
main:
        pushq   %rbp
        movq    %rsp, %rbp
        pushq   $8
        pushq   $7
        movl    $6, %r9d
        movl    $5, %r8d
        movl    $4, %ecx
        movl    $3, %edx
        movl    $2, %esi
        movl    $1, %edi
        call    suma
        addq    $16, %rsp
        movl    %eax, %esi
        movl    $str, %edi
        movl    $0, %eax
        call    printf
        movl    $0, %eax
        leave
        ret

suma:
        pushq   %rbp
        movq    %rsp, %rbp
        movl    %edi, -4(%rbp)
        movl    %esi, -8(%rbp)
        movl    %edx, -12(%rbp)
        movl    %ecx, -16(%rbp)
        movl    %r8d, -20(%rbp)
        movl    %r9d, -24(%rbp)
        movl    -4(%rbp), %edx
        movl    -8(%rbp), %eax
        addl    %eax, %edx
        movl    -12(%rbp), %eax
        addl    %eax, %edx
        movl    -16(%rbp), %eax
        addl    %eax, %edx
        movl    -20(%rbp), %eax
        addl    %eax, %edx
        movl    -24(%rbp), %eax
        addl    %eax, %edx
        movl    16(%rbp), %eax
        addl    %eax, %edx
        movl    24(%rbp), %eax
        addl    %edx, %eax
        popq    %rbp
        ret
