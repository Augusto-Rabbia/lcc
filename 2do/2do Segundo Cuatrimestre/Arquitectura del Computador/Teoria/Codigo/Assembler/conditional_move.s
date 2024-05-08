.global main
main:
        movq $-1, %rbx
        subq %rax, %rax     # Z=1
        cmovzq %rbx, %rax   # Se realiza el momiviento porque Z=1
        ret
