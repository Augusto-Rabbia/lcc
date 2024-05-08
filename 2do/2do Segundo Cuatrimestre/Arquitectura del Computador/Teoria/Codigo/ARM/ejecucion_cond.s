.global main
main:
	mov r0, #0
	mov r1, #10
	cmp r0, #0
	addeq r1, r1, r0
	movne r1, #1
	bx lr
