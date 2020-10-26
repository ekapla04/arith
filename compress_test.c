#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "assert.h"
#include "pnm.h"
#include "a2methods.h"
#include "a2plain.h"

#include "compress40.h"
#include "compression.h"

typedef struct closure {
    unsigned denominator;
    A2Methods_UArray2 array;
} *closure;


/*
Gustavo's notes:
- CVS_array is storing the struct itself, not a pointer to the struct
- Make sure storing pixmap correctly (when trimming)
*/

/* * * * * * * * * * Function declarations * * * * * * * * * * * */


void RGB_to_CVS(int col, int row, A2Methods_UArray2 src_img,
                A2Methods_Object *cvs_elem, void *cl);

Pnm_CVS convert_RGB(Pnm_rgb RGB, unsigned denominator);


void CVS_to_RGB(int col, int row, A2Methods_UArray2 src_img,
                A2Methods_Object *cvs_elem, void *cl);

Pnm_rgb convert_CVS(Pnm_CVS CVS);

void free_array(int col, int row, A2Methods_UArray2 src_img,
                A2Methods_Object *cvs_elem, void *cl);

extern void decompress40(FILE *input) 
{
    unsigned height, width;
    int read = fscanf(input, "COMP40 Compressed image format 2\n%u %u",
                      &width, &height);
    assert(read == 2);
    int c = getc(input);
    assert(c == '\n');
    // printf("width: %u, height: %u\n", width, height);
    // printf("hello\n");
    // Pnm_ppm pixmap = Pnm_ppmread(input);
    // assert(pixmap);
}

extern void compress40(FILE *input)
{
    printf("\n     ---STARTING COMPRESSION---\n");
    A2Methods_T methods = uarray2_methods_plain; 
    assert(methods);

    Pnm_ppm pixmap = Pnm_ppmread(input, methods);
    assert(pixmap);


    /* * * * * * * * * * * * convert to CVS * * * * * * * * * * */

    A2Methods_UArray2 CVS_array = methods->new(pixmap->width, pixmap->height,
                                        sizeof(struct Pnm_CVS));

    closure cl = malloc(sizeof(struct closure));
    cl->denominator = pixmap->denominator;
    cl->array = CVS_array;
    // free closure;

    methods->map_row_major(pixmap->pixels, RGB_to_CVS, cl);

    // Pnm_CVS CVS_values = malloc(sizeof(struct Pnm_CVS)); 

    // CVS_values->PR = 0;
    // CVS_values->Y = 0;
    // CVS_values->PB = 0;

    // free(CVS_values);

    methods->free(&(pixmap->pixels));

    pixmap->pixels = CVS_array;
    // methods->map_row_major(pixmap->pixels, free_array, NULL);  
    printf("between\n");
    methods->free(&(pixmap->pixels));
    printf("out of mapping\n");
    // free(CVS_array);
    free(pixmap);
    pixmap = NULL;
    free(cl);
    cl = NULL;
    // Pnm_ppmfree(&pixmap);
}


A2Methods_UArray2 RGB_test(int width, int height, Pnm_ppm pixmap){
    A2Methods_T methods = uarray2_methods_plain; 
    A2Methods_UArray2 VCS_array = methods->new(pixmap->width, pixmap->height,
                                sizeof(struct Pnm_rgb));

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            *((Pnm_rgb*)methods->at(VCS_array, i, j)) = *((Pnm_rgb*)methods->at(pixmap->pixels, i, j));
        }
        
    }
    return VCS_array;
}

void RGB_to_CVS(int col, int row, A2Methods_UArray2 src_img,
                A2Methods_Object *rgb_elem, void *cl_struct)
{
    (void)src_img;
    fprintf(stderr, "\ncol: %d and row: %d\n", col, row);
    
    A2Methods_T methods = uarray2_methods_plain; 
    A2Methods_UArray2 CVS_array = ((closure)cl_struct)->array;
    unsigned denominator = ((closure)cl_struct)->denominator;
    
    //  printf("\nconverting col: %d and row: %d\n", col, row);
    
    Pnm_CVS CVS_values = convert_RGB((Pnm_rgb)rgb_elem, denominator);
    // Pnm_CVS CVS_values = malloc(sizeof(struct Pnm_CVS));

    *((Pnm_CVS)methods->at(CVS_array, col, row)) = CVS_values;
    // printf("CVS values methods - %f", (*dest)->Y);
    // printf("CVS values - Y: %f, PB: %f, PR: %f \n", CVS_values->Y, CVS_values->PB, CVS_values->PR);

}


void CVS_to_RGB(int col, int row, A2Methods_UArray2 src_img,
                A2Methods_Object *cvs_elem, void *RGB_array)
{
/* src_img holds Pnm_CVS struct, cl/destination holds rgb struct */
    (void)src_img;
    fprintf(stderr, "\ncol: %d and row: %d\n", col, row);

    A2Methods_T methods = uarray2_methods_plain; 
    RGB_array = (A2Methods_UArray2)RGB_array;

    Pnm_rgb RGB_values = convert_CVS((Pnm_CVS)cvs_elem);
    // fprintf(stderr, "r: %u, g: %u, b: %u\n", RGB_values->red, RGB_values->green, RGB_values->blue);

    Pnm_rgb *dest = methods->at(RGB_array, col, row);
    *dest = RGB_values; 
    
    //fprintf(stderr, "r: %u, g: %u, b: %u\n", (*dest)->red, (*dest)->green, (*dest)->blue);

}

Pnm_rgb convert_CVS(Pnm_CVS CVS)
{
    Pnm_rgb RGB_values = malloc(sizeof(struct Pnm_rgb));
    

    /* WE CHOOSE DENOMINATOR TO BE 255 */
    float y = (float)CVS->Y;
    float pb = (float)CVS->PB;
    float pr = (float)CVS->PR;
    printf("CVS values - Y: %f, PB: %f, PR: %f \n", y, pb, pr);
    RGB_values->red = ((1.0 * y) + (0.0 * pb) + (1.402 * pr)) * 255;
    RGB_values->green = ((1.0 * y) - (0.344136 * pb) - (0.714136 * pr)) * 255;
    RGB_values->blue = ((1.0 * y) + (1.772 * pb) + (0.0 * pr)) * 255;
    
    printf("Y: %f, PB: %f, PR: %f --->", y, pb, pr);
    printf("original R: %u, G: %u, B: %u\n", RGB_values->red, RGB_values->green, RGB_values->blue);

    //printf("Y: %f, PB: %f, PR: %f\n",  CVS_values->Y,  CVS_values->PB,  CVS_values->PR);
    return RGB_values;

    // TODO: Don't forget to free RGB values!
}

Pnm_CVS convert_RGB(Pnm_rgb RGB, unsigned denominator)
{
    //Pnm_CVS *CVS_values;
    Pnm_CVS CVS_values = malloc(sizeof(struct Pnm_CVS)); 

    CVS_values->PR = 0;
    CVS_values->Y = 0;
    CVS_values->PB = 0;
    // CVS_values->PR = 0;

    // float r = (float)RGB->red/denominator;
    // float b = (float)RGB->blue/denominator;
    // float g = (float)RGB->green/denominator;

    // CVS_values->Y =  (0.299 * r) + (0.587 * g) + (0.114 * b);

    // CVS_values->PB = (-0.168736 * r) - (0.331264 * g) +  (0.5 * b);

    // CVS_values->PR = (0.5 * r) - (0.418688 * g) - (0.081312 * b);

    // printf("R: %f, G: %f, B: %f ---> ", r, g, b);
    // printf("Y: %f, PB: %f, PR: %f\n", CVS_values->Y, CVS_values->PB, CVS_values->PR);

    return CVS_values;
}

void free_array(int col, int row, A2Methods_UArray2 src_img,
                A2Methods_Object *cvs_elem, void *cl)
{
    (void)col;
    (void)row;
    // Pnm_CVS temp = (Pnm_CVS)cvs_elem;
    (void)src_img;
    (void)cl; //NULL

    free((Pnm_CVS)cvs_elem);
}