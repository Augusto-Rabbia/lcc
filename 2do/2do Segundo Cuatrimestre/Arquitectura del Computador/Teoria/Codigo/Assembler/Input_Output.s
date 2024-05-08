.data
str1: .asciz "Ingrese un número:\n"
str2: .asciz "%d"
str3: .asciz "El número ingresado es: %d\n"
x: .long 0

.text
.global main
main:
        pushq %rbp          # mantiene el stack alineado
        
        leaq str1, %rdi     # cadena de formato
        movq $0, %rax       # limpiar AL (cero FP args en XMM registros XMM)
        call printf         # se llama a printf
        
        leaq str2, %rdi     # cadena de formato
        leaq x, %rsi        # dirección de x
        movq $0, %rax       # limpiar AL (cero FP args en XMM registros XMM)
        call scanf          # se llama a scanf
        
        leaq str3, %rdi     # cadena de formato 
        movq x, %rsi        # valor de x en rdi
        movq $0, %rax       # limpiar AL (cero FP args en XMM registros XMM)
        call printf         # se llama a printf
        
        movq $0, %rax       # valor de retorno
        
        pop %rbp            # se restaura el valor de rsp
        ret
