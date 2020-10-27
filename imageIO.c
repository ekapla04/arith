#include "imageIO.h"


Pnm_ppm read_pixmap(FILE *input)
{
    A2Methods_T methods = uarray2_methods_plain; 

    Pnm_ppm pixmap = Pnm_ppmread(input, methods);
    assert(pixmap && methods);

    valid_dimensions(pixmap);

    return pixmap;
}

void valid_dimensions(Pnm_ppm pixmap)
{
    assert(pixmap->width >= 2 && pixmap->height >= 2);

    int width = pixmap->width;
    int height = pixmap->height;
    bool trim = false;

    if (pixmap->width % 2 != 0) { 
        width = width - 1;
        trim = true;
    }
    if (pixmap->height % 2 != 0) {
        height = height - 1;
        trim = true;
    }
    if (trim == true) {
        trim_image(pixmap, width, height);
    }
}

void trim_image(Pnm_ppm pixmap, int width, int height)
{
    A2Methods_T methods = uarray2_methods_plain; 

    A2Methods_UArray2 square_array = methods->new(width, height,
                                            sizeof(struct Pnm_rgb));
    

    methods->map_row_major(pixmap->pixels, copy_array, square_array);
    methods->free(&(pixmap->pixels));
    pixmap->pixels = square_array;
    pixmap->width = width;
    pixmap->height = height;
}

void copy_array(int col, int row, A2Methods_UArray2 src_img,
                A2Methods_Object *rgb_elem, void *dest_img)
{

    (void)src_img;

    A2Methods_T methods = uarray2_methods_plain; 
    dest_img = (A2Methods_UArray2)dest_img;

    int width = methods->width(dest_img);
    int height = methods->height(dest_img);
    if (col < width && row < height) {
        *((Pnm_rgb *)methods->at(dest_img, col, row)) = *((Pnm_rgb *)rgb_elem);        
    }
}