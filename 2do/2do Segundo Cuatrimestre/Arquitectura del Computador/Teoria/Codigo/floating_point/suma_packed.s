.data
.align 16
a: .double 3.0, 2.0 
b: .double 5.0, 7.0
c: .double 0.0

.text
.global main
main:
	movapd a, %xmm0  # carga 2 dobles desde a
	addpd b, %xmm0   # suma a[0]+b[0] y a[1]+b[1]
	movapd %xmm0, c  # guarda las dos sumas en c
	xor	%eax, %eax
	ret