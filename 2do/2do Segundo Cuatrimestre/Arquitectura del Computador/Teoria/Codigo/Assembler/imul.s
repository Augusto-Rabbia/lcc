.globl main
main:
        movl $-1,%eax   # Solo para este tama~no el mov pone en 0
                        # la parte alta del registro.
        movl $2, %ecx
        imull %ecx
        salq $32, %rdx
        orq %rdx, %rax
        ret

