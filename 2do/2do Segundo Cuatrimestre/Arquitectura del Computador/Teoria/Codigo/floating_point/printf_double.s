.data
format: .asciz "%f\n"
g: .double 3.14


.text
.global main
main:
    pushq %rbp         # Prólogo
    movq %rsp, %rbp
    
    movq $format, %rdi  # El primer argumento es el formato.
    movsd g, %xmm0
    movq $1, %rax       # Cantidad de valores de punto flotante.
    call printf
    
    #movq %rbp, %rsp     # Epílogo
    #popq %rbp
    ret



