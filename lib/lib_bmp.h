#include <stdio.h>

typedef struct bmp_header {
    char BM[2];
    unsigned int size_Byte;
    unsigned int offset_Byte;
} bmp_header_t;

void print_bmp(bmp_header_t *bmp);
