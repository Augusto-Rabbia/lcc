.data
f:      .long   0x4048f5c3
str:    .string "%f\n"
        
.text
.global  main
main:
        pushq   %rbp
        movq    %rsp, %rbp
        movl    $2, -4(%rbp)
        movl    $3, %eax
        cvtsi2ss   %eax, %xmm0
        movss   %xmm0, -8(%rbp)
        pxor    %xmm0, %xmm0
        cvtsi2ss       -4(%rbp), %xmm0
        movss  %xmm0, %xmm1
        mulss   -8(%rbp), %xmm1
        movss   f, %xmm0
        addss   %xmm1, %xmm0
        cvtss2sd        %xmm0, %xmm0
        movsd   %xmm0, -16(%rbp)
        movq    -16(%rbp), %rax
        movq    %rax, %xmm0
        movq    $str, %rdi
        movl    $1, %eax
        call    printf
        movl    $0, %eax
        leave
        ret


