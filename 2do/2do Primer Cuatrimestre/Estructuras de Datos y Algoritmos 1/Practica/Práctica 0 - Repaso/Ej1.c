#include <stdlib.h>
#include <stdio.h>

void set_first(int *array[], int num)
{
    //printf("%d", (sizeof(array)/sizeof(int)));
    array[0] = num;
    printf("Ej2: ");
    for(int i=0; i<= 5; i++){
        printf("%d, ", array[i]);
    }
}

void set_first2(int *array[], int num)
{
    *array[0] = num;
    
}

int main()
{
    int a = 1, b = 5;
    char letra1 = 'H', str[] = "Hola";
    printf("Ej1: %p, %p, %p, %p\n\n", &a, &b, &letra1, &str);

    int arr_nums[] = {1, 2, 3, 4, 5};
    set_first(arr_nums, 3);
    printf("\n");
    for(int i=0; i<= (sizeof(arr_nums)/sizeof(int)); i++){
        printf("%d, ", arr_nums[i]);
    }
    printf("\n\n");

    //printf("Ej3:");
    //int arr_nums2[] = {1, 2, 3, 4, 5};
    //set_first2(arr_nums2, 3);


    

    return 0;
}