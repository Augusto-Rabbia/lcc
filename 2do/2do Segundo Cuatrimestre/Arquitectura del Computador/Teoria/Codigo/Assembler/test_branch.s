.global main
main:
	mov r0, #1
	cmp r0, #0
	beq dest
	bx lr
dest:
	mov r0, #2
	bx lr
	
