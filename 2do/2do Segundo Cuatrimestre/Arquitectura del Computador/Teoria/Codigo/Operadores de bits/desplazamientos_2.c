#include<stdio.h>
int main(){
        char a=-32;
        char c=a>>2;
        printf("a=%d\n",a);
        printf("a/4=%d\n",c);
        c=a>>6;
        printf("a/64=%d\n",c);
        return 0;
}
