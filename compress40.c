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
} closure;


/*
Gustavo's notes:
- CVS_array is storing the struct itself, not a pointer to the struct
- Make sure storing pixmap correctly (when trimming)
*/

/* * * * * * * * * * Function declarations * * * * * * * * * * * */
void copy_array(int col, int row, A2Methods_UArray2 src_img,
                A2Methods_Object *rgb_elem, void *dest_array);

void RGB_to_CVS(int col, int row, A2Methods_UArray2 array2,
                A2Methods_Object *ptr, void *cl_struct);

Pnm_CVS convert_RGB(Pnm_rgb RGB, unsigned denominator);

void CVS_to_RGB(int col, int row, A2Methods_UArray2 src_img,
                A2Methods_Object *cvs_elem, void *cl);

Pnm_rgb convert_CVS(Pnm_CVS CVS);

void trim_image(Pnm_ppm pixmap, int width, int height);

void valid_dimensions(Pnm_ppm pixmap);

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

    Pnm_ppm pixmap = Pnm_ppmread(input, methods);
    assert(pixmap && methods);
    printf("pre valid dimensions - width: %d, height: %d\n", pixmap->width, pixmap->height);
    valid_dimensions(pixmap);
    printf("post valid dimensions - width: %d, height: %d\n", pixmap->width, pixmap->height);

    /* * * * * * * * * * * * convert to CVS * * * * * * * * * * */
    A2Methods_UArray2 CVS_array = methods->new(pixmap->width, pixmap->height,
                                        sizeof(Pnm_CVS));

    closure cl;
    cl.denominator = pixmap->denominator;
    cl.array = CVS_array;

    methods->map_row_major(pixmap->pixels, RGB_to_CVS, &cl);

    methods->free(&(pixmap->pixels));
    pixmap->pixels = CVS_array;
    printf("CVS dimensions - width: %d, height: %d\n", pixmap->width, pixmap->height);
    //Pnm_ppmwrite(stdout, pixmap); --> CAN't tell if it works from this line

    /* * * * * * * * * * * * * * compress by block * * * * * * * * * * */
    printf("width: %d, height: %d", methods->width(CVS_array), methods->height(CVS_array));
    compress_by_block(CVS_array);

    /* * * * * * * * * * * * * * REVERSION * * * * * * * * * * */

    // printf("\n     ---STARTING REVERSION---\n");
    // //NOTE: only for testing, will move to decompression function, and get 
    // //       width and height from header
    // A2Methods_UArray2 square_array2 = methods->new(pixmap->width, pixmap->height,
    //                                     sizeof(struct Pnm_rgb));

    // methods->map_row_major(pixmap->pixels, CVS_to_RGB, square_array2); 
    // methods->free(&(pixmap->pixels));
    // pixmap->pixels = square_array2;
    // printf("sq array dimensions - width: %d, height: %d\n", pixmap->width, pixmap->height);


    Pnm_ppmfree(&pixmap);
}

void valid_dimensions(Pnm_ppm pixmap)
{
    assert(pixmap->width >= 2 && pixmap->height >= 2);

    int width = pixmap->width;
    int height = pixmap->height;
    printf("valid dimensions - width: %d, height: %d\n", pixmap->width, pixmap->height);
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
    printf("trim dimensions - width: %d, height: %d\n", pixmap->width, pixmap->height);
}

void copy_array(int col, int row, A2Methods_UArray2 src_img,
                A2Methods_Object *rgb_elem, void *dest_img)
{

    (void)src_img;

    A2Methods_T methods = uarray2_methods_plain; 
    dest_img = (A2Methods_UArray2)dest_img;

    int width = methods->width(dest_img);
    int height = methods->height(dest_img);
    if (col < width && row < height){
        *((Pnm_rgb *)methods->at(dest_img, col, row)) = *((Pnm_rgb *)rgb_elem);        
    }
}

void RGB_to_CVS(int col, int row, A2Methods_UArray2 src_img,
                A2Methods_Object *rgb_elem, void *cl_struct)
{
    (void)src_img;
    // fprintf(stderr, "\ncol: %d and row: %d\n", col, row);
    
    A2Methods_T methods = uarray2_methods_plain; 
    A2Methods_UArray2 CVS_array = ((closure *)cl_struct)->array;
    unsigned denominator = ((closure *)cl_struct)->denominator;
    
    Pnm_CVS CVS_values = convert_RGB((Pnm_rgb)rgb_elem, denominator);

    *((Pnm_CVS *)methods->at(CVS_array, col, row)) = CVS_values;
    // printf("CVS values - Y: %f, PB: %f, PR: %f \n", CVS_values.Y, CVS_values.PB, CVS_values.PR);

}


Pnm_CVS convert_RGB(Pnm_rgb RGB, unsigned denominator)
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


void CVS_to_RGB(int col, int row, A2Methods_UArray2 src_img,
                A2Methods_Object *cvs_elem, void *RGB_array)
{
    (void)src_img;
    // fprintf(stderr, "\ncol: %d and row: %d\n", col, row);

    A2Methods_T methods = uarray2_methods_plain; 
    RGB_array = (A2Methods_UArray2)RGB_array;

    Pnm_rgb RGB_values = convert_CVS(*(Pnm_CVS *)cvs_elem);
    fprintf(stderr, "r: %u, g: %u, b: %u\n", RGB_values->red, RGB_values->green, RGB_values->blue);

    Pnm_rgb *dest = methods->at(RGB_array, col, row);
    *dest = RGB_values; 
    
    fprintf(stderr, "r: %u, g: %u, b: %u\n", (*dest)->red, (*dest)->green, (*dest)->blue);

}

Pnm_rgb convert_CVS(Pnm_CVS CVS)
{
    Pnm_rgb RGB_values = malloc(sizeof(struct Pnm_rgb));

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