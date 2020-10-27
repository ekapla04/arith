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
    assert(cvs_elem != NULL && RGB_array != NULL);
    (void)src_img;

    A2Methods_T methods = uarray2_methods_plain; 
    RGB_array = (A2Methods_UArray2)RGB_array;

    struct Pnm_rgb RGB_values = make_RGB(*(Pnm_CVS *)cvs_elem);
   
    *((struct Pnm_rgb *)methods->at(RGB_array, col, row)) = RGB_values;

}

struct Pnm_rgb make_RGB(Pnm_CVS CVS)
{
    struct Pnm_rgb RGB_values;
    long double red, green, blue = 0;

    long double y = (long double)CVS.Y;
    long double pb = (long double)CVS.PB;
    long double pr = (long double)CVS.PR;

    red = (1.0 * y) + (0.0 * pb) + (1.402 * pr);
    green = (1.0 * y) - (0.344136 * pb) - (0.714136 * pr);
    blue = (1.0 * y) + (1.772 * pb) + (0.0 * pr);

    RGB_values.red = round(round_f(red, 0, 1) * 255);
    RGB_values.green = round(round_f(green, 0, 1) * 255);
    RGB_values.blue = round(round_f(blue, 0, 1) * 255);

    return RGB_values;

}