.data
valFloat: .word 0x3fe00000      @ 1.75
valFloat2: .word 0x40000000     @ 2.0

.text
.global main
main:
        ldr r1, =valFloat       @ En r1 queda la direccion de valFloat
        vldr s0, [r1] @         En s0 queda el valor 1.75
        ldr r1, =valFloat2      @ En r1 queda la direccion de valFloat2
        vldr s1, [r1]           @ En s1 queda el valor 2.0
        vmul.f32 s2, s0, s1     @ s2=s0*s1=3.5
        vmul.f32 s2, s2, s2     @ s2=s2*s2=12.25
        vcvt.s32.f32 s0, s2     @ Conversion de punto flotante a entero (s0=12)
        vmov r0, s0             @ Copia del registro s0 al registro r0 (r0=12)
        bx lr                   @ Retorna r0=12
