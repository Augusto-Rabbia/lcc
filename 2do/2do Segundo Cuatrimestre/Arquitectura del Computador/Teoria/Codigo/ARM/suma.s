.global main
main:
	sub     sp, sp, #8
     movs    r3, #5
     str     r3, [sp, #4]
     movs    r3, #7
     str     r3, [sp]
     ldr     r2, [sp, #4]
     ldr     r3, [sp]
     add     r3, r3, r2
     str     r3, [sp, #4]
     ldr     r3, [sp, #4]
     mov     r0, r3
     add     sp, sp, #8
     bx      lr