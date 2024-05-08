.data
str: .space 4

.text
.global main
main:
		mov r1, #1
		mov r2, #2
		mov r3, #3
		mov r4, #4
		mov r5, #5
		
		ldr r0, =str
        stmia r0!, {r1-r5}
		
		ldr r0, =str
		stmib r0!, {r1-r5}
		
		ldr r0, =str
		stmda r0!, {r1-r5}
		
		ldr r0, =str
		stmdb r0!, {r1-r5}
   		bx lr