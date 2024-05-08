#include <stdio.h>
#include <stdlib.h>



int main(){
    int hundredfirst[100];
    for(int i=1;i <= sizeof(hundredfirst)/sizeof(hundredfirst[i]); i++){
        hundredfirst[i] = i;
        printf("%d\n", hundredfirst[i]);
    }
    
    system("Pause");
    return 0;
}