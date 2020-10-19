#include "compress40.h"
#include <stdlib.h>
#include <stdio.h>
#include "pnm.h"
#include "assert.h"
#include "a2methods.h"
#include "a2plain.h"
#include "stdbool.h"
#include "string.h"

typedef struct brightness {
    float Y;
    float PB;
    float PR;
} brightness; 

void RGB_to_VCS(int col, int row, A2Methods_UArray2 array2,
                A2Methods_Object *ptr, void *cl);

Pnm_ppm create_ppm(Pnm_ppm src_image);

brightness *covert_RGB(Pnm_rgb RGB);

extern void compress40(FILE *input)
{
    // printf("heyo\n");
    A2Methods_T methods = uarray2_methods_plain; 
    assert(methods);
    Pnm_ppm pixmap = Pnm_ppmread(input, methods);
    assert(pixmap);
    if (pixmap->width % 2 != 0) {
        pixmap->width = pixmap->width - 1;
    }
    if (pixmap->height % 2 != 0) {
        pixmap->height = pixmap->height - 1;
    }
    printf("struct size: %ld\n", sizeof(struct brightness));
    printf("width: %d\n", pixmap->width);
    printf("height: %d\n", pixmap->height);
    A2Methods_UArray2 VCS_array = methods->new(pixmap->width, pixmap->height,
                                                sizeof(struct brightness));
    Pnm_ppm VCS_pixmap = create_ppm(pixmap);
    methods->map_row_major(pixmap->pixels, RGB_to_VCS, &VCS_pixmap);
    Pnm_ppmfree(&VCS_pixmap);
    Pnm_ppmfree(&pixmap);
}

Pnm_ppm create_ppm(Pnm_ppm src_image)
{
    assert(src_image != NULL);
    int width = src_image->width;
    int height = src_image->height;
    int size = src_image->methods->size(src_image->pixels);

    Pnm_ppm final_image = malloc(sizeof(*final_image));
    assert(final_image != NULL);
    final_image->width = width;
    final_image->height = height;
    final_image->denominator = src_image->denominator;
    final_image->methods = src_image->methods;
    final_image->pixels = final_image->methods->new(width, height, size);

    return final_image;
}

void RGB_to_VCS(int col, int row, A2Methods_UArray2 array2,
                A2Methods_Object *ptr, void *cl)
{
    brightness *VCS_values = malloc(sizeof(struct brightness));
    // come back to... doesnt like us dereferencing ptr --> thinks its void
    // try casting as RGB pnm 
    VCS_values = convert_RGB(*ptr);
    Pnm_ppm VCS_pixmap = cl;
    brightness *dest = (VCS_pixmap->methods)->at(VCS_pixmap->pixels, col, row);
    *dest = *VCS_values;
    // ((brightness *)pixmap->methods->at(pixmap->pixels, col, row)) = ptr;
    (void)col;
    (void)row;
    (void)array2;
    (void)ptr;
    (void)cl;
    printf("hey\n");
    // eventually free VCS values
}

brightness *covert_RGB(Pnm_rgb RGB)
{
    brightness VCS_values; 
    VCS_values.Y =  0.299 * RGB->red + 0.587 * RGB->green + 0.114 * RGB->blue;
    VCS_values.PB = -0.168736 * RGB->red - 0.331264 * RGB->green + 0.5 * RGB->blue;
    VCS_values.PR = 0.5 * RGB->red - 0.418688 * RGB->green - 0.081312 * RGB->blue;
    printf("Y: %d, PB: %d, PR: %d\n",  VCS_values.Y,  VCS_values.PB,  VCS_values.PR);
    return VCS_values;
}

extern void decompress40(FILE *input) 
{
    unsigned height, width;
    int read = fscanf(input, "COMP40 Compressed image format 2\n%u %u",
                      &width, &height);
    assert(read == 2);
    int c = getc(input);
    assert(c == '\n');
    printf("width: %u, height: %u\n", width, height);
    // printf("hello\n");
    // Pnm_ppm pixmap = Pnm_ppmread(input);
    // assert(pixmap);
}