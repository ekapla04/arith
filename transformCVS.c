/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*                           transformCVS.c                             *
*                                                                      * 
*       Assignment: arith                                              *
*       Authors: Emily Gjertsson (egjert01) & Elise Kaplan (ekapla04)  *  
*       Date: 10/26/2020                                               *
*                                                                      *
*       Summary                                                        *
*           All functions needed to transform an CVS array to one      *
*           full of corresponding Pnm_rgb structs                      *
*                                                                      *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#include "transformCVS.h"

long double round2(long double to_round, long double low, long double high)
{
    if (to_round < low) {
        to_round = low;
    } else if (to_round > high) {
        to_round = high;
    }
    return to_round;
}

void CVS_to_RGB(int col, int row, A2Methods_UArray2 src_img,
                A2Methods_Object *cvs_elem, void *RGB_array)
{
    (void)src_img;
    // fprintf(stderr, "\ncol: %d and row: %d\n", col, row);

    A2Methods_T methods = uarray2_methods_plain; 
    RGB_array = (A2Methods_UArray2)RGB_array;

    struct Pnm_rgb RGB_values = make_RGB(*(Pnm_CVS *)cvs_elem);
    printf("CVS_to_RGB r: %u, g: %u, b: %u\n", RGB_values.red, RGB_values.green, RGB_values.blue);

    // struct Pnm_rgb *dest = methods->at(RGB_array, col, row);
    // *dest = RGB_values; 
    *((struct Pnm_rgb *)methods->at(RGB_array, col, row)) = RGB_values;

    
    //fprintf(stderr, "r: %u, g: %u, b: %u\n", (*dest)->red, (*dest)->green, (*dest)->blue);

}

struct Pnm_rgb make_RGB(Pnm_CVS CVS)
{
    struct Pnm_rgb RGB_values;
    long double red, green, blue;
    /* WE CHOOSE DENOMINATOR TO BE 255 */
    long double y = (long double)CVS.Y;
    long double pb = (long double)CVS.PB;
    long double pr = (long double)CVS.PR;

    printf("make_RGB Unpacked CVS values - Y: %Lf, PB: %Lf, PR: %Lf \n", y, pb, pr);

    red = (1.0 * y) + (0.0 * pb) + (1.402 * pr);
    green = (1.0 * y) - (0.344136 * pb) - (0.714136 * pr);
    blue = (1.0 * y) + (1.772 * pb) + (0.0 * pr);

    RGB_values.red = round_f(red, 0, 1) * 255;
    RGB_values.green = round_f(green, 0, 1) * 255;
    RGB_values.blue = round_f(blue, 0, 1) * 255;

        
    // printf("MAKE RGB, from Y: %Lf, PB: %Lf, PR: %Lf --->", y, pb, pr);
    // printf(" R: %u, G: %u, B: %u\n", RGB_values.red, RGB_values.green, RGB_values.blue);

    // printf("rounded: %Lf \n", round(RGB_values.red));

    //printf("Y: %f, PB: %f, PR: %f\n",  CVS_values->Y,  CVS_values->PB,  CVS_values->PR);
    return RGB_values;

}

// print result of math -- go through each equation for compression & decompression
// CVS->RGB and luminance values (quantization), chroma
// compare