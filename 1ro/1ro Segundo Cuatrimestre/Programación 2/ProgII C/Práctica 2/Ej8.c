#include <stdio.h>
#include <stdlib.h>



int main(){
    int steps, current;
    printf("Give an integer to try the Collatz Conjucture: ");
    scanf("%d", &current);
    for(steps = 0;current != 1;steps ++){
        if(current < 1){
            printf("Error.\n"); 
            break;
        }
        else if(current % 2 == 0){
            current = current/2;
            printf("Next step: %d\n", current);
        }
        else{current = current * 3 + 1;
            printf("Next step: %d\n", current);}
    }
    printf("Final value: %d, steps: %d\n", current, steps);
    system("Pause");
    return 0;
}