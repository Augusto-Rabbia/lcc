.data
    str: .asciz "123456"
.text
.global search_str
search_str:
    pushq %rbp
    movq %rsp, %rbp

    xorq %rax, %rax
iter_search_str:
    movb (%rdi, %rax), %dl
    cmpb %sil, %dl
    je salir
    cmpb $0, %dl
    je no_encontrado_search_str
    incq %rax
    jmp iter_search_str

no_encontrado_search_str:
    movq $-1, %rax
salir:
    leaveq
    ret

.global search_str2
search_str2:
    pushq %rbp
    movq %rsp, %rbp

    movb %sil, %dl
    movq %rdi, %rsi
    movb %dl, %dil
    xorq %rdx, %rdx
iter_search_str2:
    lodsb
    cmpb %dil, %al
    je salir2
    cmpb $0, %al
    je no_encontrado_search_str2
    incq %rdx
    jmp iter_search_str2

no_encontrado_search_str2:
    movq $-1, %rax
    leaveq
    ret
salir2:
    movq %rdx, %rax
    leaveq
    ret

.global main
main:
    pushq %rbp
    movq %rsp, %rbp

    movq $str, %rdi
    movb $'8',%sil

    call search_str2

    leaveq
    ret
