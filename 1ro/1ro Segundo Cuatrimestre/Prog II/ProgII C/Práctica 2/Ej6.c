#include <stdio.h>
#include <stdlib.h>



int main(){
    printf("Guess an integer between 1 and 500!\n");
    int guess, attempts = 15, answer = 200;
    for(int i = attempts; i >= 0; i--){
        printf("Make a guess, ypu have %d attempts left: ", i);
        scanf("%d", &guess);
        if(guess == answer){
            printf("You won!\n");
            break;
            }
        if(i == 0){printf("No attempts left, you lose.\n");}
    }
    system("Pause");
    return 0;
}