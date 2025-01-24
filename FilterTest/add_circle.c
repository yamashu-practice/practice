#include <stdio.h>
#include <stdlib.h>
#include "common.h"

int main(void){
    Image img;
    int i, j; //counter
    int x, y; //点の座標
    const int r = 32; //円の半径

    int result = input_img(&img);
    if (result != SUCCESS){
        exit(1);
    }

    for (i = HEIGHT / 4; i <= HEIGHT * 3 / 4; i++){
        y = HEIGHT / 2 - i;
        for (j = WIDTH / 4; j <= WIDTH * 3 / 4; j++){
            x = WIDTH / 2 - j;
            if (x * x + y * y <= r * r) {
                img.data[i * WIDTH + j] = 255;
            }
        }
    }

    output_img(&img);

    return 0;
}