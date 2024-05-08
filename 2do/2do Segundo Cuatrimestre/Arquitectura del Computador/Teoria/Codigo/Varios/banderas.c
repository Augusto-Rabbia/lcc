#include<stdio.h>
int main(){
        int a=0x7fffffff;
        int b;
        b=a+a;
        int c=0x80000000;
        int d;
        d=c+c;
        int e=0x0fffffff;
        int f;
        f=e+e;
        int g=0xffffffff;
        int h;
        h=g+g;
        return 0;
}
