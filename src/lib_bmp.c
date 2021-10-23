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
    bmp_hdr->size_total_b = bmp_size;

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
    if (fread(&(bmp_hdr->bpp), 2, 1, fp) != 1) {
        return 2;
    }

    if (bmp_hdr->bpp != PIXEL_BITS_SIZE) {
        fprintf(stderr,"bpp of %u is currently unsupported.", bmp_hdr->bpp);
        return 2;
    }

    /* Read compress */
    fseek(fp, 30, SEEK_SET);
    if (fread(&(bmp_hdr->compress), 4, 1, fp) != 1) {
        return 2;
    }

    if (bmp_hdr->compress != 0) {
        fprintf(stderr,"compress algo of %u is currently unsupported. :p", bmp_hdr->bpp);
        return 2;
    }

    /* Read bitmap size */
    fseek(fp, 34, SEEK_SET);
    if (fread(&(bmp_hdr->size_bitmap), 4, 1, fp) != 1) {
        return 2;
    }

    rewind(fp);

    return 0;
}

void
bmp_print_header(bmp_header_t *bmp)
{
    printf("BMP HEADER:\n");
    printf("HEAD: %s\n", bmp->BM);
    printf("SIZE: %u\n", bmp->size_total_b);
    printf("OFFSET: %u\n", bmp->offset_b);
    printf("WIDTH: %u\n", bmp->width);
    printf("HEIGHT: %u\n", bmp->height);
    printf("BPP: %u\n", bmp->bpp);
    printf("COMPRESS: %u\n", bmp->compress);
    printf("BITMAP_SIZE: %u\n", bmp->size_bitmap);
}

int
bmp_pixel_read(int offset, int pixel_sz_bits, bmp_pixel_t *bmp_pixel, FILE *fp)
{
    /* Assuming 24-bit AXRGB notation */
    /* pixel bits are 3 bytes, 24 bits */ 
    unsigned int bmp_pixel_bits = 0;
    
    if (pixel_sz_bits != PIXEL_BITS_SIZE)
        return 0;
    size_t ret;

    fseek(fp, offset, SEEK_SET);

    if ((ret = fread(&bmp_pixel_bits, pixel_sz_bits/8, 1, fp)) != 1) {
        //fprintf(stderr,"Failed to read pixel\n");
        return 0;
    }

    DEBUG_PRINT("PIXEL FOUND: %x\n", bmp_pixel_bits);
    DEBUG_PRINT("offset: %d\n", offset);
    DEBUG_PRINT("ret: %ld\n", ret);

    unsigned int r_mask = 0xFF0000;
    unsigned int g_mask = 0x00FF00;
    unsigned int b_mask = 0x0000FF;

    bmp_pixel->r     = (r_mask & bmp_pixel_bits) >> 16;
    bmp_pixel->g     = (g_mask & bmp_pixel_bits) >> 8;
    bmp_pixel->b     = (b_mask & bmp_pixel_bits);


    DEBUG_PRINT("R: %x %d\n", bmp_pixel->r, bmp_pixel->r);
    DEBUG_PRINT("G: %x %d\n", bmp_pixel->g, bmp_pixel->g);
    DEBUG_PRINT("B: %x %d\n", bmp_pixel->b, bmp_pixel->b);

    return ret;
}

void
bmp_pixel_init(bmp_pixel_t *bmp_pixel, unsigned char r, unsigned char g, unsigned char b)
{
    bmp_pixel->b = b;
    bmp_pixel->g = g;
    bmp_pixel->r = r;
}

void
bmp_convert_pixel_to_gs(bmp_pixel_t *pxl)
{
    if ( (pxl->r == 255) && (pxl->b == 255) && (pxl->g == 255) )
        return;
    /* NTSC Formula */
    unsigned char red = pxl->r * 0.299;
    unsigned char blu = pxl->b * 0.114;
    unsigned char grn = pxl->g * 0.587;

    unsigned char biggest;
    biggest = (red > grn) ? red : grn;
    biggest = (biggest > blu) ? biggest : blu;

    bmp_pixel_init(
        pxl,
        biggest,
        biggest,
        biggest
    );
}

void
bmp_convert_to_gs(bmp_header_t *bmp, FILE *fp, FILE *fout)
{
    unsigned char byt;
    size_t ret, ret_o, ret_p;
    unsigned long index = 0;

    /* Copy Header */
    ret = fread(&byt, sizeof(byt), 1, fp);
    ret_o = 1;
    while ((ret) && (index < bmp->offset_b) && (ret_o)) {
        ret_o = fwrite(&byt, sizeof(byt), 1, fout);
        index += ret_o;
        DEBUG_PRINT(" %ld ", index);
        ret = fread(&byt, sizeof(byt), 1, fp);
    }
    printf("\n");

    int pixel_array_size = bmp->width * bmp->height;
    int pixel_array_end = bmp->offset_b + pixel_array_size * 4; 
    DEBUG_PRINT("ENDPXL: %i\n", pixel_array_end);
    bmp_pixel_t bmp_pixel;
    DEBUG_PRINT("SIZEOFPXL: %li\n", sizeof(bmp_pixel));

    int bytes_in_pixel = bmp->bpp / 8;
    int num_pixels_till_pad = 2;
    unsigned short pad_bytes = 0;
    unsigned long pixels_written = 0;

    /* Copy Pixel */
    ret_p = bmp_pixel_read(bmp->offset_b, bmp->bpp, &bmp_pixel, fp);
    ret_o = 1;
    while ((ret_p) && (index < pixel_array_end) && (ret_o)) {
        if (num_pixels_till_pad > 0) {
            /* Write Converted Pixel */
            bmp_convert_pixel_to_gs(&bmp_pixel);
            DEBUG_PRINT("R: %x %d\n", bmp_pixel.r, bmp_pixel.r);
            DEBUG_PRINT("G: %x %d\n", bmp_pixel.g, bmp_pixel.g);
            DEBUG_PRINT("B: %x %d\n", bmp_pixel.b, bmp_pixel.b);
            ret_o = fwrite(&bmp_pixel, bytes_in_pixel, 1, fout);
            num_pixels_till_pad -= 1;
            index += sizeof(bmp_pixel);
            pixels_written += 1;
        } else {
            /* Write Padding */
            ret = fread(&pad_bytes, sizeof(pad_bytes), 1, fp);
            DEBUG_PRINT("PADDING %x\n", pad_bytes);
            num_pixels_till_pad = 2;
            ret_o = fwrite(&pad_bytes, sizeof(pad_bytes), 1, fout);
            index += sizeof(pad_bytes);
        }
        DEBUG_PRINT(" %ld ", index);
        if (pixels_written < pixel_array_size) {
            ret_p = bmp_pixel_read(index, bmp->bpp, &bmp_pixel, fp);
        } else {
            break;
        }
    }

    /* Copy End */
    ret = fread(&byt, sizeof(byt), 1, fp);
    DEBUG_PRINT("ret %li\n %li", ret, sizeof(byt));
    ret_o = 1;
    while ((ret) && (index < bmp->size_total_b) && (ret_o)) {
        ret_o = fwrite(&byt, sizeof(byt), 1, fout);
        index += ret_o;
        DEBUG_PRINT(" %ld ", index);
        ret = fread(&byt, sizeof(byt), 1, fp);
    }

}
