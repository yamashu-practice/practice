#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int get_next_non_space_char(FILE *fp);
int get_next_token(FILE* fp, char *token, size_t size);
int get_next_int(FILE* fp);
int parse_int(const char *str);
unsigned char normalize(int value, int max);
int read_pnm_stream(FILE* fp, Image *img);
int read_p1(FILE *fp, Image* img);
int read_p2(FILE *fp, Image* img);
int read_p3(FILE *fp, Image* img);
int read_p4(FILE *fp, Image* img);
int read_p5(FILE *fp, Image* img);
int read_p6(FILE *fp, Image* img);

int input_img(Image *img){
    //ファイル名格納用
    const int buffer_size = 50;
    char file_name[buffer_size];
    FILE *fp;

    //ファイル名を指定してファイルを開く
    printf("Input file name to read: ");
    scanf("%s", file_name);
    if ((fp = fopen(file_name, "rb")) == NULL) {
        printf("Error: Couldn't file open\n");
        exit(1);
    }
    printf("File opened...\n");
    if(read_pnm_stream(fp, img) != SUCCESS){
        fclose(fp);
        return FAILURE;
    }
    fclose(fp);
    return SUCCESS;
}

int get_next_non_space_char(FILE *fp){
    int character = 'a';
    int is_comment = FALSE;
    while (character != EOF){
        character = getc(fp);
        if (is_comment) {
            if (character == '\n' || character == '\r') {
                is_comment = FALSE;
            }
            continue;
        }
        if (character == '#') {
            is_comment = TRUE;
            continue;
        }
        if (!isspace(character)) {
            break;
        }
    }
    return character;
}

int get_next_token(FILE* fp, char *token, size_t size){
    int i = 0;
    int character = get_next_non_space_char(fp);
    while(character != EOF && !isspace(character) && i < size - 1){
        token[i] = character;
        i++;
        character = getc(fp);
    }
    return i;
}

int get_next_int(FILE* fp){
    char token[11]; //11桁あればintの最大値を格納可能
    get_next_token(fp, token, sizeof(token));
    return parse_int(token);
}

int parse_int(const char *str){
    int i;
    int result = 0;
    if (str[0] == 0){
        return -1;
    }
    for (i = 0; str[i] != 0; i++){
        if (!isdigit((int)str[i])) {
            return -1;
        }
        result *= 10;
        result += str[i] - '0';
    }
    return result;
}

unsigned char normalize(int value, int max){
    int min;
    min = (value < max) ? value : max;
    return (min * 255 + max / 2)  / max;
}

int read_pnm_stream(FILE* fp, Image *img){
    char token[4]; //ヘッダー文字列用バッファ
    int type = 0; //PNMタイプ（P1～6）
    int width = 0;
    int height = 0;
    int max = 0; //最大輝度
    
    //ヘッダーの読み取り
    get_next_token(fp, token, sizeof(token));
    type = token[1] - '0'; //char->int変換
    if (token[0] != 'P' || type < 1 || type > 6 || token[2] != '\0') {
        return FAILURE;
    }
    printf("type is %d.\n",type);
    width = get_next_int(fp);
    printf("width is %d.\n",width);
    height = get_next_int(fp);
    if (width <= 0 || height <= 0) {
        return FAILURE;
    }
    if (type != 1 && type != 4) {
        max = get_next_int(fp);
        if (max < 1 || max > 65535) {
            return FAILURE;
        }
    }

    //初期化
    img->width = width;
    img->height = height;
    img->max_value = max;

    printf("width: %d, height: %d, max: %d\n", width, height, max);

    switch (type)
    {
    case 1:
        //ASCII 2値
        img->num_bit = 1;
        img->data = (unsigned char*)malloc(sizeof(char) * (width * height));
        if (img->data == NULL) {
            return FAILURE;
        }
        if (read_p1(fp, img) != SUCCESS){
            free(img-> data);
            return FAILURE;
        }
        break;
    case 2:
        //ASCII グレースケール
        img->num_bit = 1;
        img->data = (unsigned char*)malloc(sizeof(char) * (width * height));
        if (img->data == NULL) {
            return FAILURE;
        }
        if (read_p2(fp, img) != SUCCESS){
            free(img-> data);
            return FAILURE;
        }
        break;
    case 3:
        //ASCII RGB
        img->num_bit = 3;
        img->data = (unsigned char*)malloc(sizeof(char) * (width * height) * 3);
        if (img->data == NULL) {
            return FAILURE;
        }
        if (read_p3(fp, img) != SUCCESS){
            free(img-> data);
            return FAILURE;
        }
        break;
    case 4:
        //バイナリ 2値
        img->num_bit = 1;
        img->data = (unsigned char*)malloc(sizeof(char) * (width * height));
        if (img->data == NULL) {
            return FAILURE;
        }
        if (read_p4(fp, img) != SUCCESS){
            free(img-> data);
            return FAILURE;
        }
        break;
    case 5:
        //バイナリ グレースケール
        img->num_bit = 1;
        img->data = (unsigned char*)malloc(sizeof(char) * (width * height));
        if (img->data == NULL) {
            return FAILURE;
        }
        if (read_p5(fp, img) != SUCCESS){
            free(img-> data);
            return FAILURE;
        }
        break;
    case 6:
        //バイナリ RGB
        img->num_bit = 3;
        img->data = (unsigned char*)malloc(sizeof(char) * (width * height) * 3);
        if (img->data == NULL) {
            return FAILURE;
        }
        if (read_p6(fp, img) != SUCCESS){
            free(img-> data);
            return FAILURE;
        }
        break;
    default:
        return FAILURE;
        break;
    }

    return SUCCESS;
}

int read_p1(FILE *fp, Image* img){
    int x, y;
    int tmp;
    for (y = 0; y < img->height; y++){
        for (x = 0; x < img->width; x++){
            tmp = get_next_non_space_char(fp);
            if (tmp == '0') {
                img->data[y * img->width + x] = 0;
            } else if (tmp == '1') {
                img->data[y * img->width + x] = 1;
            } else {
                return FAILURE;
            }
        }
    }
    return SUCCESS;
}

int read_p2(FILE *fp, Image* img){
    int x, y;
    int tmp;
    for (y = 0; y < img->height; y++){
        for (x = 0; x < img->width; x++){
            tmp = get_next_int(fp);
            if (tmp < 0){
                return FAILURE;
            }
            img->data[y * img->width + x] = normalize(tmp, img->max_value);
        }
    }
    return SUCCESS;
}

int read_p3(FILE *fp, Image* img){
    int x, y;
    int color; //0~2、RGB3色に対応
    int tmp;
    for (y = 0; y < img->height; y++){
        for (x = 0; x < img->width; x++){
            tmp = get_next_int(fp);
            if (tmp < 0){
                return FAILURE;
            }
            for (color = 0; color < 3; color++){
                img->data[y * img->width + x + color] = 
                                        normalize(tmp, img->max_value);
            }
        }
    }
    return SUCCESS;
}

int read_p4(FILE *fp, Image *img){
    int x, y;
    unsigned char *row; //一列の情報をいれるバッファ
    int stride; //一列が何バイトか
    int pos = 0; //一列の中のどのバイトに演算をするか
    int shift = 0; //何ビットシフトするか
    //たとえばあるバイトについて5bit目を見たい場合
    // 10110011 -> (4bit shift) -> 00001011 -> (&1) -> 00000001

    stride = (img->width + 7) / 8;
    row = malloc(stride);
    if (row == NULL){
        return FAILURE;
    }

    for (y = 0; y < img->height; y++){
        pos =0;
        shift = 8;
        if (fread(row, stride, 1, fp) != 1){
            free(row);
            return FAILURE;
        }
        for (x = 0; x < img->width; x++){
            shift--;
            img->data[y * img->width + x] = (row[pos] >> shift) & 1;
            if (shift == 0){
                shift = 8;
                pos++;
            }
        }
    }
    free(row);
    return SUCCESS;
}

int read_p5(FILE *fp, Image *img){
    int x, y;
    int tmp;
    unsigned char *row;
    int stride;
    int bpc = img->max_value > 255 ? 2 : 1;
    stride = img->width * bpc;
    row = malloc(stride);
    if (row == NULL){
        return FAILURE;
    }
    for (y = 0; y < img->height; y++){
        if (fread(row, stride, 1, fp) != 1){
            free(row);
            return FAILURE;
        }
        if (bpc == 1) {
            for (x = 0; x < img->width; x++){
                img->data[y * img->width + x] = 
                                normalize(row[x], img->max_value);
            }
        } else {
            for (x = 0; x < img->width; x++){
                tmp = row[x * bpc] << 8; //10110110 -> 10110110 00000000
                tmp = tmp | row[x * bpc + 1];
                img->data[y * img->width + x] =
                                normalize(tmp, img->max_value);
            }
        }
    }
    free(row);
    return SUCCESS;
}

int read_p6(FILE *fp, Image *img){
    int x, y;
    int tmp;
    unsigned char *row;
    int stride;
    int bpc = img->max_value > 255 ? 2 : 1;
    int color; //RGB3色に対応、0~2
    stride = img->width * 3 * bpc;
    row = malloc(stride);
    if (row == NULL){
        return FAILURE;
    }
    for (y = 0; y < img->height; y++){
        if (fread(row, stride, 1, fp) != 1){
            free(row);
            return FAILURE;
        }
        if (bpc == 1) {
            for (x = 0; x < img->width; x++){
                for (color = 0; color < 3; color++){
                    img->data[y * img->width + x] =
                        normalize(row[x + color], img->max_value);
                }
            }
        } else {
            for (x = 0; x < img->width; x++){
                for (color = 0; color < 3; color++){
                    tmp = row[x * bpc + color] << 8;
                    tmp = tmp | row[x * bpc + color + 1];
                    img->data[y * img->width + x] =
                        normalize(tmp, img->max_value);
                }
            }
        }
    }

    free(row);
    return FAILURE;
}