/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*                           transformRGB.c                             *
*                                                                      * 
*       Assignment: arith                                              *
*       Authors: Emily Gjertsson (egjert01) & Elise Kaplan (ekapla04)  *  
*       Date: 10/26/2020                                               *
*                                                                      *
*       Summary                                                        *
*           All functions needed to transform an RGB array to one      *
*           full of corresponding Pnm_CVS structs                      *
*                                                                      *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#include "transformRGB.h"

long double round_f(long double to_round, long double low, long double high){
    if (to_round < low){
        to_round = low;
    } else if (to_round > high){
        to_round = high;
    }
    return to_round;
}

void convert_to_CVS(Pnm_ppm pixmap)
{
    assert(pixmap != NULL);

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
    assert(src_img != NULL && cl_struct != NULL && rgb_elem != NULL);
    
    A2Methods_T methods = uarray2_methods_plain; 
    A2Methods_UArray2 CVS_array = ((closure *)cl_struct)->array;
    unsigned denominator = ((closure *)cl_struct)->denominator;
    
    Pnm_CVS CVS_values = make_CVS(*((Pnm_rgb)rgb_elem), denominator);

    *((Pnm_CVS *)methods->at(CVS_array, col, row)) = CVS_values;

}

Pnm_CVS make_CVS(struct Pnm_rgb RGB, unsigned denominator)
{
    Pnm_CVS CVS_values; 

    long double r = (long double)RGB.red;
    long double b = (long double)RGB.blue;
    long double g = (long double)RGB.green;

    CVS_values.Y =  ((0.299 * r) + (0.587 * g) + (0.114 * b)) / 
                    (long double)denominator;

    CVS_values.PB = ((-0.168736 * r) - (0.331264 * g) +  (0.5 * b)) / 
                    (long double)denominator;

    CVS_values.PR = ((0.5 * r) - (0.418688 * g) - (0.081312 * b)) / 
                    (long double)denominator;

    CVS_values.Y = round_f(CVS_values.Y, 0, 1);
    CVS_values.PB = round_f(CVS_values.PB, -0.5, 0.5);
    CVS_values.PR = round_f(CVS_values.PR, -0.5, 0.5);

    return CVS_values;
}