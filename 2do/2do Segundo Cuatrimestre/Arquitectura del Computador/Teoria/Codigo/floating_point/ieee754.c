
// Ejemplo usando la cabecera ieee754.h
#include<stdio.h>
#include<math.h>
#include<ieee754.h>

int main(){
        union ieee754_float a, b, c;
        a.f=-3.14;
        b.f=NAN;
        c.f=INFINITY;
        printf("-----------------------------\n");
        printf("a=%f\n",a.f);
        printf("Mantisa=%x\n",a.ieee.mantissa);
        printf("Exponente=%x\n",a.ieee.exponent);
        printf("Signo=%x\n",a.ieee.negative);
        printf("-----------------------------\n");
        printf("b=%f\n",b.f);
        printf("Mantisa=%x\n",b.ieee.mantissa);
        printf("Exponente=%x\n",b.ieee.exponent);
        printf("Signo=%x\n",b.ieee.negative);
        printf("-----------------------------\n");
        printf("c=%f\n",c.f);
        printf("Mantisa=%x\n",c.ieee.mantissa);
        printf("Exponente=%x\n",c.ieee.exponent);
        printf("Signo=%x\n",c.ieee.negative);
        printf("-----------------------------\n");
        return 0;
}
