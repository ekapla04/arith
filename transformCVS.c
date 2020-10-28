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

/* * * * * * * * private function declarations * * * * * * * * */
struct Pnm_rgb make_RGB(Pnm_CVS CVS);
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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

/* 
 * Purpose: Helper function that makes instance of RGB struct and calculates
 *          color values from CVS values
 * Arguments: Pnm_CVS 
 * Returns: Pnm_rgb struct
 * Notes: Performs range check on RGB values and rounds them
 * Mem alloc: None
 * C.R.E.: None
 */
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

    RGB_values.red = round(range_check(red, 0, 1) * DENOMINATOR);
    RGB_values.green = round(range_check(green, 0, 1) * DENOMINATOR);
    RGB_values.blue = round(range_check(blue, 0, 1) * DENOMINATOR);

    return RGB_values;

}