#include <stdio.h>

#define DEBUG 1

#define DEBUG_PRINT(fmt, ...) \
    do { if (DEBUG) fprintf(stderr, fmt, __VA_ARGS__); } while(0)

typedef struct bmp_header {
    char BM[2];
    unsigned int size_b;
    unsigned int offset_b;
    signed int width;
    signed int height;
    unsigned int bpp;
} bmp_header_t;

typedef struct bmp_pixel {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char alpha;
} bmp_pixel_t;

typedef struct bmp_img
{
    bmp_header_t header;
    bmp_pixel_t **pixels;
} bmp_img_t;

int bmp_header_init(bmp_header_t *bmp_hdr, FILE *fp); 
void bmp_pixel_init(bmp_pixel_t *bmp_pixel, 
                    unsigned char r, 
                    unsigned char b,
                    unsigned char g,
                    unsigned char alpha
                    );

void bmp_print_header(bmp_header_t *bmp);
int bmp_pixel_read(int offset, int pixel_sz_bits, bmp_pixel_t *bmp_pixel, FILE *fp);
void bmp_convert_to_gs(bmp_header_t *bmp, FILE *fp);
