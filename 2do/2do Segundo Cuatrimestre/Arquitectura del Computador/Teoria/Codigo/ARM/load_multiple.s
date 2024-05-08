.data
a: .word 0x01, 0x02, 0x03

.text
.global main
main:
        ldr r0, =a
        mov r1, #0
        mov r2, #0
        mov r3, #0

        ldmia r0!, {r1-r3}

        bx lr
