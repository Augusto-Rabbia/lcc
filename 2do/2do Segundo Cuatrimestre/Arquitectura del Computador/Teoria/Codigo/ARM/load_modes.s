.data
a: .word 0x01010101, 0x02020202

.text
.global main
main:
        ldr r1, =a
        ldr r0, [r1, #4]!       @ preindex with writeback

        ldr r1, =a
        ldr r0, [r1, #4]        @ preindex

        ldr r1, =a
        ldr r0, [r1], #4        @ postindex

        bx lr
