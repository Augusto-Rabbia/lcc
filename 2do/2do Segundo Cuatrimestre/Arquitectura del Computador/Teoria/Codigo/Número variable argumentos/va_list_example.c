#include <stdarg.h>
#include <stdio.h>
double average(int n,...);

int main()
{
    printf("%f\n", average(5,3.0,2.0,1.0,2.0,1.0));
    printf("%f\n", average(3,3.0,2.0,1.0));
    printf("%f\n", average(2,3.0,2.0));
    return 0;
}

double average(int n,...){
    va_list l;
    double sum=0;
    va_start(l, n);
    for(int i=0; i<n; i++){
        sum+=va_arg ( l, double ); 
    }
    va_end(l);
    return sum/n;
}

