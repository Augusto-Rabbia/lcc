#include <stdio.h>
#include <ieee754.h>

int main(){
	union ieee754_float mif;
	mif.ieee.exponent = 128;
	mif.ieee.negative = 0;
	mif.ieee.mantissa = 0;
	printf("%f\n", mif.f);
	return 0;
} 
