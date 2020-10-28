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

/* * * * * * * * * * * * * private members * * * * * * * * * * */
Pnm_CVS make_CVS(struct Pnm_rgb RGB, unsigned denominator);

void RGB_to_CVS(int col, int row, A2Methods_UArray2 array2,
                A2Methods_Object *ptr, void *cl_struct);

/* struct used to pass in two parameters in the closure */
typedef struct closure {
    unsigned denominator;
    A2Methods_UArray2 array;
} closure;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


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

/* 
 * Purpose: Apply function, called by map_row_major in convert_to_CVS, visits 
 *          every element in the pixmap array storing Pnm_rgb structs 
 * Arguments: Pnm_ppm pixmap
 * Returns: None (void)
 * Mem alloc: helper function allocates space for CVS_array, which 
 *            replaces the pixmap->pixels after freeing pixmap->pixels
 * C.R.E.:  raised if pixmap is NULL
 */
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

/* private function */
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

    CVS_values.Y = range_check(CVS_values.Y, 0, 1);
    CVS_values.PB = range_check(CVS_values.PB, -0.5, 0.5);
    CVS_values.PR = range_check(CVS_values.PR, -0.5, 0.5);

    return CVS_values;
}