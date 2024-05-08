.data
a: .float 1.0, 2.0, 3.0, 4.0

.text
.global main
main:
	ldr r1, =a
	vldr.f32 s0, [r1]
	vldr.f32 s1, [r1, #4]
	vldr.f32 s2, [r1, #8]
	vldr.f32 s3, [r1, #12]
	vldr.f64 d3, [r1]
	bx lr