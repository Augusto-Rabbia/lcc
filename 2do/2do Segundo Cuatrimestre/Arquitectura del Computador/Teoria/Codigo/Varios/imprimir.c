/*Este ejemplo muestra cómo ingresar datos en diferentes sistemas de numeración y cómo mostrarlos por pantalla
*/
#include<stdio.h>
int main(){
        int a=45;
        int b=0x2d;
        int c=055;
        int d=0b101101;

        printf("%d\n",a);
        printf("%d\n",b);
        printf("%d\n",c);
        printf("%d\n",d);

        printf("%x\n",a);
        printf("%x\n",b);
        printf("%x\n",b);
        printf("%x\n",d);

        printf("%X\n",a);
        printf("%X\n",b);
        printf("%X\n",c);
        printf("%X\n",d);

        printf("%o\n",a);
        printf("%o\n",b);
        printf("%o\n",c);
        printf("%o\n",d);
        return 0;
}
