/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
*                           transformCVS.c
*                               
*       Assignment: arith
*       Authors: Emily Gjertsson (egjert01) & Elise Kaplan (ekapla04)    
*       Date: 10/26/2020
*
*       Summary
*           All functions needed to transform a CVS array to one
*           full of corresponding Pnm_rgb structs 
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "transformCVS.h"

float round2(float to_round, float low, float high){
    if (to_round < low){
        to_round = low;
    } else if (to_round > high){
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
    // fprintf(stderr, "r: %u, g: %u, b: %u\n", RGB_values->red, RGB_values->green, RGB_values->blue);

    // struct Pnm_rgb *dest = methods->at(RGB_array, col, row);
    // *dest = RGB_values; 
    *((struct Pnm_rgb *)methods->at(RGB_array, col, row)) = RGB_values;

    
    //fprintf(stderr, "r: %u, g: %u, b: %u\n", (*dest)->red, (*dest)->green, (*dest)->blue);

}

struct Pnm_rgb make_RGB(Pnm_CVS CVS)
{
    struct Pnm_rgb RGB_values;
    float red, green, blue;
    /* WE CHOOSE DENOMINATOR TO BE 255 */
    float y = (float)CVS.Y;
    float pb = (float)CVS.PB;
    float pr = (float)CVS.PR;

    // printf("CVS values - Y: %f, PB: %f, PR: %f \n", y, pb, pr);

    red = (1.0 * y) + (0.0 * pb) + (1.402 * pr);
    green = (1.0 * y) - (0.344136 * pb) - (0.714136 * pr);
    blue = (1.0 * y) + (1.772 * pb) + (0.0 * pr);

    RGB_values.red = round_f(red, 0, 1) * 255;
    RGB_values.green = round_f(green, 0, 1) * 255;
    RGB_values.blue = round_f(blue, 0, 1) * 255;

        
    // printf("MAKE RGB, from Y: %f, PB: %f, PR: %f --->", y, pb, pr);
    // printf("original R: %u, G: %u, B: %u\n", RGB_values.red, RGB_values.green, RGB_values.blue);

    //printf("Y: %f, PB: %f, PR: %f\n",  CVS_values->Y,  CVS_values->PB,  CVS_values->PR);
    return RGB_values;

}