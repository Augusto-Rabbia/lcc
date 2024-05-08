.global main
main:
	mov r0, #0xaa 	@ alguna instrucción
	bl calculo
	mov r1, #0xbb	@ otra instrucción
	bx lr
	
calculo:
	add r0, r0, #10 @ algún cálculo
	bx lr
