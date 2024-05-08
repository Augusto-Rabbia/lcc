.data
a: .word 0xee, 0xaa

.text
.global main
main:
	ldr r1, =a			@ en r1 queda la dirección de a
	ldr r2, [r1]		@ en r2 queda el valor de a
	add r2, r2, #1		@ se incrementa en uno a r2
	str r2, [r1, #4]	@ se guarda el valor incrementado en a+4
	bx lr