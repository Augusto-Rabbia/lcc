#include <stdio.h>
#include <stdlib.h>


int main(){
    int NumArray[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99};
    int input = 1;
    int RepeatedElements[]= {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    printf("The positive integers you will input will be counted until you enter a negative integer.\n\n");
    while(input > 0){
        printf("Add a positive integer: ");
        scanf("%d", &input);
        if(input <= 99){
            for(int i = 0; i <= 100; i++){
                if(NumArray[i] == input){
                    RepeatedElements[i] ++;
                    break;
                }
            }
        }
    }
    int TimesRepeated = 0;
    int MostRepeatedNum;
    for(int i = 0; i <= sizeof(RepeatedElements)/sizeof(RepeatedElements[i]); i++){
        if(RepeatedElements[i] > TimesRepeated){
            TimesRepeated = RepeatedElements[i];
            MostRepeatedNum = NumArray[i]; 
        }
    }
    printf("The most repeated element was %d and it was inputted %d times\n", MostRepeatedNum, TimesRepeated);
    system("Pause");
    return 0;
}