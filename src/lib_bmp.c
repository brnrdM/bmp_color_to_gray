#include <stdio.h>
#include "lib/lib_bmp.h"

void
print_bmp(bmp_header_t *bmp)
{
    printf("HEAD: %s\n", bmp->BM);
    printf("SIZE: %u\n", bmp->size_Byte);
    printf("OFFSET: %u\n", bmp->offset_Byte);
}
