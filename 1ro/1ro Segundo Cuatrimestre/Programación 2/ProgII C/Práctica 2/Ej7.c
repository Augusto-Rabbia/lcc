#include <stdio.h>
#include <stdlib.h>



int main(){
    char str[] = "bucle";
    char letters = 'A';
    for(int i = 5; i >= 1; i--){
        printf("%.2c %d %c\n", str[0,5-i], i, letters);
    }
    
    system("Pause");
    return 0;
}