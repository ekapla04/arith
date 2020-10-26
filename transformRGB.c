#include "transformRGB.h"

void convert_to_CVS(Pnm_ppm pixmap)
{
    A2Methods_T methods = uarray2_methods_plain; 
    A2Methods_UArray2 CVS_array = methods->new(pixmap->width, pixmap->height,
                                        sizeof(Pnm_CVS));

    closure cl;
    cl.denominator = pixmap->denominator;
    cl.array = CVS_array;

    methods->map_row_major(pixmap->pixels, RGB_to_CVS, &cl);

    methods->free(&(pixmap->pixels));
    pixmap->pixels = CVS_array;
    
}

void RGB_to_CVS(int col, int row, A2Methods_UArray2 src_img,
                A2Methods_Object *rgb_elem, void *cl_struct)
{
    (void)src_img;
    // fprintf(stderr, "\ncol: %d and row: %d\n", col, row);
    
    A2Methods_T methods = uarray2_methods_plain; 
    A2Methods_UArray2 CVS_array = ((closure *)cl_struct)->array;
    unsigned denominator = ((closure *)cl_struct)->denominator;
    
    Pnm_CVS CVS_values = make_CVS((Pnm_rgb)rgb_elem, denominator);

    *((Pnm_CVS *)methods->at(CVS_array, col, row)) = CVS_values;
    // printf("CVS values - Y: %f, PB: %f, PR: %f \n", CVS_values.Y, CVS_values.PB, CVS_values.PR);

}

Pnm_CVS make_CVS(Pnm_rgb RGB, unsigned denominator)
{
    Pnm_CVS CVS_values; 

    float r = (float)RGB->red/denominator;
    float b = (float)RGB->blue/denominator;
    float g = (float)RGB->green/denominator;

    CVS_values.Y =  (0.299 * r) + (0.587 * g) + (0.114 * b);

    CVS_values.PB = (-0.168736 * r) - (0.331264 * g) +  (0.5 * b);

    CVS_values.PR = (0.5 * r) - (0.418688 * g) - (0.081312 * b);

    // printf("R: %f, G: %f, B: %f ---> ", r, g, b);
    // printf("Y: %f, PB: %f, PR: %f\n", CVS_values.Y, CVS_values.PB, CVS_values.PR);

    return CVS_values;
}