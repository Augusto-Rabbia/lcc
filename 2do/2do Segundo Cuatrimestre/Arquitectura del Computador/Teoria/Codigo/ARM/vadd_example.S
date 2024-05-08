.data
a: .float 1.0, 2.0
b: .float 1.0, 2.0

.text
.global main
main:
		ldr r0, =a
		vldr.f64 d0, [r0]
		ldr r0, =b
		vldr.f64 d1, [r0]
		vadd.f32 s4, s2, s0
		vadd.f32 s5, s3, s1
		bx lr
