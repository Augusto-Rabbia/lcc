.data
    list: .quad 10, 20, 30, 40, 50, 60, 70, 80, 90, 100
    length: .byte 10
.text
.global list_invert
list_invert:
    pushq %rbp
    movq %rsp, %rbp
    
    xorq %rcx, %rcx
    movb %sil, %cl
loop_list_invert1:
    pushq -8(%rdi, %rcx, 8)
    loop loop_list_invert1
    movb %sil, %cl
loop_list_invert2:
    popq -8(%rdi, %rcx, 8)
    loop loop_list_invert2

    leaveq
    ret

.global main
main:
    pushq %rbp
    movq %rsp, %rbp

    movq $list, %rdi
    xorq %rsi, %rsi
    movb length, %sil
    call list_invert

    movq $list, %rdi

    leaveq
    ret
