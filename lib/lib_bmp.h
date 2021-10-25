#include <stdio.h>

#define DEBUG 0

#define DEBUG_PRINT(fmt, ...) \
    do { if (DEBUG) fprintf(stderr, fmt, __VA_ARGS__); } while(0)

#if 1
/* NTSC Luma Coding*/
#define LUMINOSITY_MULTIPLIER_RED   0.299
#define LUMINOSITY_MULTIPLIER_GREEN 0.587
#define LUMINOSITY_MULTIPLIER_BLUE  0.114
#elif 0
/* HDR TV Luma Coding*/
#define LUMINOSITY_MULTIPLIER_RED   0.2627
#define LUMINOSITY_MULTIPLIER_GREEN 0.6780
#define LUMINOSITY_MULTIPLIER_BLUE  0.0593
#else
/* HDTV Luma Coding*/
#define LUMINOSITY_MULTIPLIER_RED   0.2126
#define LUMINOSITY_MULTIPLIER_GREEN 0.7152
#define LUMINOSITY_MULTIPLIER_BLUE  0.0722
#endif

#define PIXEL_BITS_SIZE 24
#define PIXEL_BYTE_SIZE 3

typedef struct bmp_header {
    char BM[2];
    unsigned int size_total_b;
    unsigned int offset_b;
    signed int width;
    signed int height;
    unsigned int bpp;
    unsigned int compress;
    unsigned int size_bitmap;
} bmp_header_t;

typedef struct bmp_pixel {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} bmp_pixel_t;

typedef struct bmp_img
{
    bmp_header_t header;
    bmp_pixel_t **pixels;
} bmp_img_t;

int bmp_header_init(bmp_header_t *bmp_hdr, FILE *fp); 
void bmp_pixel_init(bmp_pixel_t *bmp_pixel, 
                    unsigned char r, 
                    unsigned char g,
                    unsigned char b
                    );

void bmp_print_header(bmp_header_t *bmp);
int bmp_pixel_read(int offset, int pixel_sz_bits, bmp_pixel_t *bmp_pixel, FILE *fp);
void bmp_convert_pixel_to_gs(bmp_pixel_t *pxl);
void bmp_convert_to_gs(bmp_header_t *bmp, FILE *fp, FILE *fout);
