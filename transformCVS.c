#include "transformCVS.h"

void CVS_to_RGB(int col, int row, A2Methods_UArray2 src_img,
                A2Methods_Object *cvs_elem, void *RGB_array)
{
    (void)src_img;
    // fprintf(stderr, "\ncol: %d and row: %d\n", col, row);

    A2Methods_T methods = uarray2_methods_plain; 
    RGB_array = (A2Methods_UArray2)RGB_array;

    Pnm_rgb RGB_values = make_RGB(*(Pnm_CVS *)cvs_elem);
    // fprintf(stderr, "r: %u, g: %u, b: %u\n", RGB_values->red, RGB_values->green, RGB_values->blue);

    Pnm_rgb *dest = methods->at(RGB_array, col, row);
    *dest = RGB_values; 
    
    //fprintf(stderr, "r: %u, g: %u, b: %u\n", (*dest)->red, (*dest)->green, (*dest)->blue);

}

Pnm_rgb make_RGB(Pnm_CVS CVS)
{
    Pnm_rgb RGB_values = malloc(sizeof(struct Pnm_rgb));
    assert (RGB_values != NULL);
    
    /* WE CHOOSE DENOMINATOR TO BE 255 */
    float y = (float)CVS.Y;
    float pb = (float)CVS.PB;
    float pr = (float)CVS.PR;
    // printf("CVS values - Y: %f, PB: %f, PR: %f \n", y, pb, pr);
    RGB_values->red = ((1.0 * y) + (0.0 * pb) + (1.402 * pr)) * 255;
    RGB_values->green = ((1.0 * y) - (0.344136 * pb) - (0.714136 * pr)) * 255;
    RGB_values->blue = ((1.0 * y) + (1.772 * pb) + (0.0 * pr)) * 255;
    
    // printf("Y: %f, PB: %f, PR: %f --->", y, pb, pr);
    // printf("original R: %u, G: %u, B: %u\n", RGB_values->red, RGB_values->green, RGB_values->blue);

    //printf("Y: %f, PB: %f, PR: %f\n",  CVS_values->Y,  CVS_values->PB,  CVS_values->PR);
    return RGB_values;

    // TODO: Don't forget to free RGB values!
}