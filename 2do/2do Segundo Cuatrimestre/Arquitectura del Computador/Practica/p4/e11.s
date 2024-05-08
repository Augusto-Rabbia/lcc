.data
    a11: .word 0x3fe00000 @ 1.75
    a12: .word 0x40000000 @ 2.0
    a21: .word 0x42000000 @ 4.0
    a22: .word 0xC0000000 @ -2.0

.text
.global det2x2
det2x2:
    VMUL.F32 s0, s0, s3
    VMUL.F32 s1, s1, s2
    VSUB.F32 s0, s0, s1

    bx lr

.global main
main:
    push {lr}

    LDR r0, =a11
    VLDR.F32 s0, [r0]

    LDR r1, =a12
    VLDR.F32 s0, [r1]

    LDR r2, =a21
    VLDR.F32 s0, [r2]

    LDR r3, =a22
    VLDR.F32 s0, [r3]

    bl det2x2

    pop {lr}
    bx lr
