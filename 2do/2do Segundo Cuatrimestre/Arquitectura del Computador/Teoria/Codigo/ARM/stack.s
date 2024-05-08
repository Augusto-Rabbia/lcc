.text
.global main
main:
		mov r4, #0xaa
		mov r5, #0xbb
		mov r6, #0xcc
		mov r7, #0xdd
		stmfd r13!, {r4-r7}		@ equivale a push {r4-r7}
		ldmfd r13!, {r4-r7} 	@ equivale a pop {r4-r7}
		bx lr
