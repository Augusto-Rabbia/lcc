.data

val: .quad 0x01234567890abcdef
hw: .asciz "hola mundo"
hw2: .ascii "hola mund"
c: .byte  'o', 0xd,10, 65,65, '!' ,0



.text
.global main
main:
	movq $val, %rax     #rax contiene la dirección en dónde está val
	movq val , %rax     #rax contiene lo que está en val
	leaq val , %rax     #Igual a la primera linea!!!
	
	addq $7, %rax
	movb (%rax),%dl
	movb -1(%rax),%dl
	movb -0x1(%rax),%dl
#	movb val(%rax), %dl #direccion val+val+7

	movq $val, %rax
	movq $2, %rcx
	movb  (%rax,%rcx,2),%dl
	movb 2(%rax,%rcx,2),%dl
	leaq 2(%rax,%rcx,2),%rdx
	lea  2(%rax,%rcx,2),%edx # ???
	movq $100, %rax
	leaq 2(,%rax,2), %rdx # ???
	
	ret
