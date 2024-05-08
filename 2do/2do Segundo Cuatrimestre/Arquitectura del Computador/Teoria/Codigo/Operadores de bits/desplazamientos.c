#include<stdio.h>
int main(){
        char a=32;
        printf("a=%d\n",a);
        printf("a*2=%d\n",a<<1);
        printf("a*4=%d\n",a<<2);
        printf("a*8=%d\n",a<<3);
        printf("a*16=%d\n",a<<4);
        char b=(a<<4)+1;
        printf("(a*16)+1=%d\n",b);
        return 0;
}

