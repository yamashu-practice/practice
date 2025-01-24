#include "common.h"
#include <stdio.h>
#include <stdlib.h>

void output_img(Image *img){
    const int buffer_size = 50;
    char file_name[buffer_size];
    FILE *fp;
    printf("Input new file name: ");
    scanf("%s", file_name);
    if ((fp = fopen(file_name, "w")) == NULL) {
        printf("Error: Couldn't file open\n");
        exit(1);
    }
    fprintf(fp, "P5\n");
    fprintf(fp, "%d %d\n", WIDTH, HEIGHT);
    fprintf(fp, "255\n");
    fclose(fp);
    if ((fp = fopen(file_name, "ab")) == NULL) {
        printf("Error: Couldn't file open\n");
        exit(1);
    }
    fwrite(img->data, sizeof(unsigned char),
            img->width * img->height, fp);
}