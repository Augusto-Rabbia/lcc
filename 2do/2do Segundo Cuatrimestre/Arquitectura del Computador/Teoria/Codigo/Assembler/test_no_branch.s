.global main
main:
	mov r0, #1
	cmp r0, #0
	moveq r0,#2
	bx lr
	
