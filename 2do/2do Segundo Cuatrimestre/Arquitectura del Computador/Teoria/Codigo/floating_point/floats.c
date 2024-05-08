#include<stdio.h>
#include<math.h>
int main(){

        float a=3.14;
        float b=2.0;
        float c=0.1;
        float d=0.0/0.0;
        float e=a/0.0;
        float i = NAN;
        float g = INFINITY;
        printf("%f\t %f\t %f\n",a,b,c);
        printf("%f\n",d);
        printf("%f\n",e);
        printf("%f\n",i);
        printf("%f\n",g);
        return 0;
}
