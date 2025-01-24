#include <stdio.h>
#define ARR_SIZE 12

void main(void){
    int array[ARR_SIZE];
    int result[ARR_SIZE];
    int kernel[] = {1, -2, 1};
    int i, j;
    for (i = 0; i < ARR_SIZE; i++){
        if (i >= 4 && i <= 8){
            array[i] = 255;
        } else {
            array[i] = 0;
        }
    }
    for (i = 0; i < ARR_SIZE - 2; i++){
        result[i] = 0;
        for(j = 0; j < 3; j++){
            result[i] += array[i + j] * kernel[j];
        }
    }
    for ( i = 0; i < ARR_SIZE; i++)
    {
        printf("pixel #%d:\t%d\t->%d\n", i, array[i], result[i]);
    }
}