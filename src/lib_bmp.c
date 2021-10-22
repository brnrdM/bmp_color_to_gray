#include <stdio.h>
#include "lib/lib_bmp.h"

int
bmp_header_init(bmp_header_t *bmp_hdr, FILE *fp)
{
    if (!fp) {
        return 1; 
    }

    unsigned short byt;
    
    /* Read Magic */
    if (fread(&byt, sizeof(byt), 1, fp) != 1) {
        return 2;
    }

    DEBUG_PRINT("byt:%u,%x\n",byt,byt);

    if (byt != 0x4d42) {
        return 2;
    }

    /* Read Size */
    unsigned int bmp_size;
    if (fread(&bmp_size, sizeof(bmp_size), 1, fp) != 1) {
        return 2;
    }
    bmp_hdr->size_b = bmp_size; 

    /* Move Cursor 4 Bytes forward*/
    fseek(fp, 4, SEEK_CUR);

    /* Read Offset */
    unsigned int bmp_offset;
    if (fread(&bmp_offset, sizeof(bmp_offset), 1, fp) != 1) {
        return 2;
    }
    bmp_hdr->offset_b = bmp_offset;
    
    /* Read width */
    fseek(fp, 18, SEEK_SET);
    if (fread(&(bmp_hdr->width), 4, 1, fp) != 1) {
        return 2;
    }

    /* Read height */
    fseek(fp, 22, SEEK_SET);
    if (fread(&(bmp_hdr->height), 4, 1, fp) != 1) {
        return 2;
    }

    /* Read bpp */
    fseek(fp, 28, SEEK_SET);
    if (fread(&(bmp_hdr->bpp), 4, 1, fp) != 1) {
        return 2;
    }

    if (bmp_hdr->bpp != 24) {
        fprintf(stderr,"bpp of %u is currently unsupported.", bmp_hdr->bpp);
        return 2;
    }

    rewind(fp);

    return 0;
}

void
bmp_print_header(bmp_header_t *bmp)
{
    printf("HEAD: %s\n", bmp->BM);
    printf("SIZE: %u\n", bmp->size_b);
    printf("OFFSET: %u\n", bmp->offset_b);
    printf("WIDTH: %u\n", bmp->width);
    printf("HEIGHT: %u\n", bmp->height);
    printf("BPP: %i\n", bmp->bpp);
}

void
bmp_pixel_init(bmp_pixel_t *bmp_pixel, unsigned char r, unsigned char b, unsigned char g, unsigned char alpha)
{
    bmp_pixel->b = b;
    bmp_pixel->g = g;
    bmp_pixel->r = r;
    bmp_pixel->alpha = alpha;
}

int
bmp_pixel_read(int offset, int pixel_sz_bits, bmp_pixel_t *bmp_pixel, FILE *fp)
{
    /* Assuming 24-bit RGBAX notation */
    unsigned int bmp_pixel_bits;

    fseek(fp, offset, SEEK_SET);
    DEBUG_PRINT("offset: %d\n", offset);

    if (fread(&bmp_pixel_bits, sizeof(bmp_pixel_bits), 1, fp) != 1) {
        return 2;
    }
    
    DEBUG_PRINT("PIXEL FOUND: %x\n", bmp_pixel_bits);

    unsigned int b_mask = 0b00000000111111111;
    bmp_pixel->b = (b_mask & bmp_pixel_bits);
    unsigned int r_mask = 0b11111111000000000;
    bmp_pixel->r = (r_mask & bmp_pixel_bits);

    DEBUG_PRINT("R: %x\n", bmp_pixel->r);
    DEBUG_PRINT("B: %x\n", bmp_pixel->b);
    DEBUG_PRINT("G: %x\n", bmp_pixel->g);

    rewind(fp);
}

void bmp_convert_to_gs(bmp_header_t *bmp, FILE *fp);
