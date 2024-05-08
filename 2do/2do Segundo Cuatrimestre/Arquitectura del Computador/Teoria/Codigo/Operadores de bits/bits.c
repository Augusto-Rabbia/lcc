#include<stdio.h>
int main(){
        char a=56, b=72;
        char c, d;
        c=a&b;
        d=a&&b;
        printf(" c=%x\n d=%d\n", c, d);
        char mask=0b00001000;
        printf("¿El cuarto bit de b es 1?:\n");
        ((b&mask)>>3)?printf("Sí\n"):printf("No\n");
        return 0;
}: