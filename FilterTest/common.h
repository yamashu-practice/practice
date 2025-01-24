#ifndef _COMMON_H_
#define _COMMON_H_

#define WIDTH 128
#define HEIGHT 128
#define TRUE 1
#define FALSE 0
#define SUCCESS 0
#define FAILURE 1

typedef struct {
    unsigned int    width;      
    unsigned int    height;
    unsigned int    num_bit;    /* チャンネル数 */
    unsigned int    max_value;  /* 最大輝度値 */
    unsigned char   *data;      /* データ部先頭アドレス */
    
} Image;

void output_img(Image *);
int input_img(Image *);

#endif //_COMMON_H_