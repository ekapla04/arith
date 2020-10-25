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
void copy_array(int col, int row, A2Methods_UArray2 src_img,
                A2Methods_Object *rgb_elem, void *dest_array);


void RGB_to_CVS(int col, int row, A2Methods_UArray2 array2,
                A2Methods_Object *ptr, void *cl_struct);

Pnm_CVS convert_RGB(Pnm_rgb RGB, unsigned denominator);


void CVS_to_RGB(int col, int row, A2Methods_UArray2 src_img,
                A2Methods_Object *cvs_elem, void *cl);

Pnm_rgb convert_CVS(Pnm_CVS CVS);

void free_array(int col, int row, A2Methods_UArray2 src_img,
                A2Methods_Object *cvs_elem, void *cl);

void standardize_RGB(int col, int row, A2Methods_UArray2 src_img,
                A2Methods_Object *rgb_elem, void *cl);

void trim_image(Pnm_ppm pixmap, bool trim_width);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
// Pnm_ppm create_ppm(Pnm_ppm src_img);

// void restore_RGB(int col, int row, A2Methods_UArray2 src_img,
//                 A2Methods_Object *rgb_elem, void *cl);

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
    A2Methods_T methods = uarray2_methods_plain; 

    Pnm_ppm pixmap = Pnm_ppmread(input, methods);
    assert(pixmap && methods);

    int width = pixmap->width;
    int height = pixmap->height;

    /* * * * * * * * * * * * * * trim image * * * * * * * * * * */
    if (pixmap->width % 2 != 0) { 
        trim_image(pixmap, true);
        //width = width - 1;
    }
    if (pixmap->height % 2 != 0) {
        trim_image(pixmap, false);
        //height = height - 1;
    }
    //TODO: WHAT if the image is less than 2x2?

    A2Methods_UArray2 square_array = methods->new(width, height,
                                            sizeof(struct Pnm_rgb));
    

    methods->map_row_major(pixmap->pixels, copy_array, square_array);
    methods->free(&(pixmap->pixels));
    //NOTE: change pixmap width and height?
    pixmap->pixels = square_array;

    printf("pixmap width: %d and height: %d\n", pixmap->width, pixmap->height);

    /* * * * * * * * * * * * convert to CVS * * * * * * * * * * */
    A2Methods_UArray2 CVS_array = methods->new(pixmap->width, pixmap->height,
                                        sizeof(struct Pnm_CVS));

    closure cl = malloc(sizeof(struct closure));
    cl->denominator = pixmap->denominator;
    cl->array = CVS_array;

printf("test1\n");
    methods->map_row_major(pixmap->pixels, RGB_to_CVS, cl);
printf("test2\n");

    methods->free(&(pixmap->pixels));
    pixmap->pixels = CVS_array;

    //Pnm_ppmwrite(stdout, pixmap); --> CAN't tell if it works from this line

    /* * * * * * * * * * * * * * REVERSION * * * * * * * * * * */

    // printf("\n     ---STARTING REVERSION---\n");
    // //NOTE: only for testing, will move to decompression function, and get 
    // //       width and height from header
    // A2Methods_UArray2 square_array2 = methods->new(pixmap->width, pixmap->height,
    //                                     sizeof(struct Pnm_rgb));

    // methods->map_row_major(pixmap->pixels, CVS_to_RGB, square_array2); 
    // methods->free(&(pixmap->pixels));
    // pixmap->pixels = square_array2;

    // printf("\nfinished reversion\n");

    /* * * * * * * * * * * * * * compress by block * * * * * * * * * * */
    //compress_by_block(CVS_array);


    /////////////////////////////////////////////////////////
    methods->map_row_major(pixmap->pixels, free_array, NULL);  
    
    //Pnm_ppmfree(&pixmap);
}
void trim_image(Pnm_ppm pixmap, bool trim_width)
{

}


/************ blocking brainstorm ***********/
/*
    - need to access 2x2 blocks at a time
    - 2x2 block:
        - C1 = (row, col), C2 = (row + 1, col)
          C3 = (row, col + 1), C4 = (row + 1, col + 1)
          0 0 0 0 
          0 0 0 0
          0 0 0 0 
          0 0 0 0
    - once we have 2x2:
        - avg chroma (C1->Y + C2->Y + C3->Y + C4->Y)
    - for (row = 0; row < width - 2; row + 2) {
            ex. index (0, 0)
            get indexes : (row, col) , (row + 1, col), (row, col + 1), (row + 1, col + 1)
            save into data structure called 2x2 block or something
            pass block into functions to manipulate it
            get next block (ie. continue for loop)
        }
*/


void copy_array(int col, int row, A2Methods_UArray2 src_img,
                A2Methods_Object *rgb_elem, void *dest_img)
{

    (void)src_img;

    A2Methods_T methods = uarray2_methods_plain; 
    dest_img = (A2Methods_UArray2)dest_img;

    int width = methods->width(dest_img);
    int height = methods->height(dest_img);
    if (col < width && row < height){
        printf("col: %d, width: %d, row: %d, height: %d\n", col, width, row, height);
        *((Pnm_rgb)methods->at(dest_img, col, row)) = *((Pnm_rgb)rgb_elem);        
    }
}

void RGB_to_CVS(int col, int row, A2Methods_UArray2 src_img,
                A2Methods_Object *rgb_elem, void *cl_struct)
{
    (void)src_img;
     printf("\ncol: %d and row: %d\n", col, row);
    
    A2Methods_T methods = uarray2_methods_plain; 
    A2Methods_UArray2 CVS_array = ((closure)cl_struct)->array;
    unsigned denominator = ((closure)cl_struct)->denominator;
    
    
    Pnm_CVS CVS_values = convert_RGB((Pnm_rgb)rgb_elem, denominator);

    // Pnm_CVS *dest = (methods)->at(CVS_array, col, row);
    // //(*dest)->Y = 5;
    // *dest = *CVS_values; //TODO: investigate

    printf("check1\n");
    *((Pnm_CVS)methods->at(CVS_array, col, row)) = *((Pnm_CVS)CVS_values);

}


Pnm_CVS convert_RGB(Pnm_rgb RGB, unsigned denominator)
{
    //Pnm_CVS *CVS_values;
    Pnm_CVS CVS_values = malloc(sizeof(struct Pnm_CVS)); 

    float r = (float)RGB->red/denominator;
    float b = (float)RGB->blue/denominator;
    float g = (float)RGB->green/denominator;

    CVS_values->Y =  (0.299 * r) + (0.587 * g) + (0.114 * b);

    CVS_values->PB = (-0.168736 * r) - (0.331264 * g) +  (0.5 * b);

    CVS_values->PR = (0.5 * r) - (0.418688 * g) - (0.081312 * b);

    // if (r != 0 || g != 0 || b != 0){
    //     printf("R: %f, G: %f, B: %f\n", r, g, b);
    //     printf("Y: %f, PB: %f, PR: %f\n",  CVS_values->Y,  CVS_values->PB,  CVS_values->PR);
    // }

    return CVS_values;
}


void CVS_to_RGB(int col, int row, A2Methods_UArray2 src_img,
                A2Methods_Object *cvs_elem, void *RGB_array)
{
/* src_img holds Pnm_CVS struct, cl/destination holds rgb struct */
    (void)src_img;
    // fprintf(stderr, "\ncol: %d and row: %d\n", col, row);

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
    float y = (float)CVS->Y*255;
    float pb = (float)CVS->PB*255;
    float pr = (float)CVS->PR*255;

    RGB_values->red = (1.0 * y) + (0.0 * pb) + (1.402 * pr);
    RGB_values->green = (1.0 * y) - (0.344136 * pb) - (0.714136 * pr);
    RGB_values->blue = (1.0 * y) + (1.772 * pb) + (0.0 * pr);
    
    // if (RGB_values->red != 0 || RGB_values->green != 0 || RGB_values->blue != 0){
    //         printf("original R: %u, G: %u, B: %u\n", RGB_values->red, RGB_values->green, RGB_values->blue);
    //         printf("Y: %f, PB: %f, PR: %f\n", y, pb, pr);
    // }

    //printf("Y: %f, PB: %f, PR: %f\n",  CVS_values->Y,  CVS_values->PB,  CVS_values->PR);
    return RGB_values;

    // TODO: Don't forget to free RGB values!
}


void free_array(int col, int row, A2Methods_UArray2 src_img,
                A2Methods_Object *cvs_elem, void *cl)
{
    // (void)col;
    // (void)row;
    (void)src_img;
    (void)cl; //NULL

    A2Methods_T methods = uarray2_methods_plain; 

    int width = methods->width(src_img);
    int height = methods->height(src_img);
    printf("width: %d, height: %d\n", width, height);
    //if (col < width && row < height - 1){
        printf("freeing %f at %d, %d\n", ((Pnm_CVS)cvs_elem)->Y, col, row);
        free((Pnm_CVS)cvs_elem);
    //}
}

/*

    (void)src_img;

    A2Methods_T methods = uarray2_methods_plain; 
    dest_img = (A2Methods_UArray2)dest_img;

    int width = methods->width(dest_img);
    int height = methods->height(dest_img);
    if (col < width && row < height){
        printf("col: %d, width: %d, row: %d, height: %d\n", col, width, row, height);
        *((Pnm_rgb)methods->at(dest_img, col, row)) = *((Pnm_rgb)rgb_elem);        
    }

*/



//////////////////////////////////////////////////////

// Pnm_ppm create_ppm(Pnm_ppm src_img)
// {
//     assert(src_img != NULL);

//     int width = src_img->width;
//     int height = src_img->height;
//     //int size = src_img->methods->size(src_img->pixels);
//     int size = (int)sizeof(struct Pnm_CVS);

//     Pnm_ppm final_image = malloc(sizeof(*final_image));
//     assert(final_image != NULL);
    
//     final_image->width = width;
//     final_image->height = height;
//     final_image->denominator = src_img->denominator;
//     final_image->methods = src_img->methods;
//     final_image->pixels = final_image->methods->new(width, height, size); 

//     return final_image;
// }

// void restore_RGB(int col, int row, A2Methods_UArray2 src_img,
//                 A2Methods_Object *rgb_elem, void *cl)
// {
//     (void)col;
//     (void)row;
//     (void)src_img;

//     unsigned *denominator = (unsigned *)cl;
//     float temp = (float)(*denominator);

//     A2Methods_T methods = uarray2_methods_plain; 

//     Pnm_rgb RGB = rgb_elem;
//     RGB->red = ((float)RGB->red * temp);
//     RGB->green = ((float)RGB->green * temp);
//     RGB->blue = ((float)RGB->blue * temp);

//     *((Pnm_rgb)methods->at(src_img, col, row)) = *RGB; 
//     printf("red: %d, blue: %d, green %d\n", RGB->red, RGB->blue, RGB->green);
// }


//TODO: STORE AS FLOATS OR THEY GET ROUNDED TO 0 OR 1
// void standardize_RGB(int col, int row, A2Methods_UArray2 src_img,
//                 A2Methods_Object *rgb_elem, void *cl)
// {
//     (void)col;
//     (void)row;
//     (void)src_img;

//     closure *cl_struct = (closure *)cl;
//     unsigned denominator = cl_struct->denominator;
//     float temp = (float)denominator;

//     char* operation = cl_struct->operation;

//     A2Methods_T methods = uarray2_methods_plain; 
//     Pnm_rgb RGB = ((Pnm_rgb) rgb_elem);

//     if (strcmp(operation, "*") == 0) {
//         RGB->red = ((float)RGB->red * temp);
//         RGB->green = ((float)RGB->green * temp);
//         RGB->blue = ((float)RGB->blue * temp);
//         printf("red_*: %f, blue_*: %d, green_*: %d\n", (float)RGB->red, RGB->blue, RGB->green);
//     }
//     else if (strcmp(operation, "/") == 0) {
//         RGB->red = ((float)RGB->red / temp);
//         RGB->green = ((float)RGB->green / temp);
//         RGB->blue = ((float)RGB->blue / temp);
//         printf("red/: %f, blue/: %d, green/: %d\n", (float)RGB->red, RGB->blue, RGB->green);
//     }
//     // printf("red_: %d, blue_o: %d, green_o: %d\n", RGB->red, RGB->blue, RGB->green);
//     *((Pnm_rgb)methods->at(src_img, col, row)) = *RGB; 
//     // losing a lot of precision --> 7/15 becomes 0.....
// }