.data
a: .float 0.0, 2.0

.text
.global main
main:
	ldr r1, =a
	vldr.f32 s0, [r1]
	vldr.f32 s1, [r1, #4]
    vcmp.f32 s0, s1
	vcmp.f32 s0, #0.0
	bx lr