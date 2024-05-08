#include<stdio.h>
int main(){
        char A=74; // (decimal)
        char B=0x4a; // (hexadecimal)
        char C=0112; // (octal)
        char D=0b01001010; // (binario)
        char E='J'; // (caracter)
        printf(" A=%d \t(decimal)\n B=%x \t(hexadecimal con minúsculas)\n B=%X \t(hexadecimal con mayúsculas)\n C=%o \t(octal)\n E=%c \t(caracter)\n",A,B,B,C,E);
        return 0;
}