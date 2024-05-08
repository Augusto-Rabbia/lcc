/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <stdio.h>

int main()
{
    float f=3.14;
    float* p= &f;
    printf("0x%x\n",*(int*) p);
    return 0;
}
