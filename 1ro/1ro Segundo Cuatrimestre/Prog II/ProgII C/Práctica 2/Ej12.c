#include <stdio.h>
#include <stdlib.h>


int main(){
    int a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int n = 10;
    for(int i = 0; i <= sizeof(a)/sizeof(a[i]); i++){
        if(a[i] == n && i!=sizeof(a)/sizeof(a[i])){
            printf("%d\n", i); 
            break;
        }
        else if(i==sizeof(a)/sizeof(a[i])){
            printf("-1\n");
        break;}
    }
    system("Pause");
    return 0;
}