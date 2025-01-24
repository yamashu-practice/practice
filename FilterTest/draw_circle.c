#include <stdio.h>
#include "common.h"

int main(void){
    unsigned char data[WIDTH * HEIGHT];
    Image *img;
    img->data = data;
    img->width = WIDTH;
    img->height = HEIGHT;
    img->num_bit = 1;
    img->max_value = 255;

    int i, j; //counter
    int x, y; //点の座標
    const int r = 32; //円の半径

    for (i = 0; i < WIDTH * HEIGHT; i++){
        img->data[i] = 0;
    }

    for (i = HEIGHT / 4; i <= HEIGHT * 3 / 4; i++){
        y = HEIGHT / 2 - i;
        for (j = WIDTH / 4; j <= WIDTH * 3 / 4; j++){
            x = WIDTH / 2 - j;
            if (x * x + y * y <= r * r) {
                img->data[i * WIDTH + j] = 100;
            }
        }
    }

    output_img(img);
    return 0;
}