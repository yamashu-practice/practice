#include <stdio.h>
#include "common.h"

int main(void){
    unsigned char data[WIDTH * HEIGHT];
    Image img;
    img.data = data;
    img.width = WIDTH;
    img.height = HEIGHT;
    img.num_bit = 1;
    img.max_value = 255;
    int i,j;

    for (i = 0; i < WIDTH * HEIGHT; i++){
        img.data[i] = 0;
    }

    for (i = HEIGHT / 3; i <= HEIGHT * 2 / 3; i++){
        for (j = WIDTH / 8; j <= WIDTH * 7 / 8; j++){
            img.data[i * WIDTH + j] = 100;
        }
    }

    output_img(&img);
    return 0;
}