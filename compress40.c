#include "compress40.h"
#include <stdlib.h>
#include <stdio.h>
#include "pnm.h"
#include "assert.h"
#include "a2methods.h"
#include "a2plain.h"
#include "stdbool.h"
#include "string.h"

typedef struct brightness {
    float Y;
    float PB;
    float PR;
} brightness; 

typedef struct partial_pnm {
    A2Methods_UArray2 aux_array;
    A2Methods_T methods;
} partial_pnm;


/* * * * * * * * * * Function declarations * * * * * * * * * * * */
void copy_array(int col, int row, A2Methods_UArray2 src_image,
                A2Methods_Object *rgb_elem, void *dest_array);


void RGB_to_VCS(int col, int row, A2Methods_UArray2 array2,
                A2Methods_Object *ptr, void *cl);

brightness *convert_RGB(Pnm_rgb RGB);


void VCS_to_RGB(int col, int row, A2Methods_UArray2 src_image,
                A2Methods_Object *vcs_elem, void *cl);

Pnm_rgb convert_VCS(brightness *VCS);


void free_array(int col, int row, A2Methods_UArray2 src_image,
                A2Methods_Object *vcs_elem, void *cl);


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
// Pnm_ppm create_ppm(Pnm_ppm src_image);


extern void compress40(FILE *input)
{
    // printf("heyo\n");
    A2Methods_T methods = uarray2_methods_plain; 
    Pnm_ppm pixmap = Pnm_ppmread(input, methods);
    assert(pixmap && methods);

    /* * * * * * * * * * * * * * trim image * * * * * * * * * * */
    if (pixmap->width % 2 != 0) { 
        pixmap->width = pixmap->width - 1;
    }
    if (pixmap->height % 2 != 0) {
        pixmap->height = pixmap->height - 1;
    }

    A2Methods_UArray2 square_array = methods->new(pixmap->width, pixmap->height,
                                            sizeof(struct Pnm_rgb));
    
    partial_pnm *cl = malloc(sizeof(struct partial_pnm));
    cl->aux_array = square_array;
    cl->methods = methods;

    methods->map_row_major(pixmap->pixels, copy_array, cl);

    methods->free(&(pixmap->pixels));
    pixmap->pixels = square_array;

    /* * * * * * * * * * * * convert to VCS * * * * * * * * * * */
    A2Methods_UArray2 VCS_array = methods->new(pixmap->width, pixmap->height,
                                        sizeof(struct brightness));
    cl->aux_array = VCS_array;

    
    // printf("width: %d, height: %d\n", pixmap->width, pixmap->height);
    //printf("R: %u, G: %u, B: %u\n", (*dest)->red, (*dest)->green, (*dest)->blue);

    methods->map_row_major(pixmap->pixels, RGB_to_VCS, cl);
    methods->free(&(pixmap->pixels));
    pixmap->pixels = VCS_array;

    
    /* * * * * * * * * * * * * * REVERSION * * * * * * * * * * */

//     printf("\n     ---STARTING REVERSION---\n");
//     //ONLY GETS THROUGH ONE ROUND BEFORE SEGFAULT!
//     A2Methods_UArray2 square_array2 = methods->new(pixmap->width, pixmap->height,
//                                         sizeof(struct Pnm_rgb));

//     cl->aux_array = square_array2;
//     methods->map_row_major(pixmap->pixels, VCS_to_RGB, cl);  
//     printf("\nfinished reversion\n");

//  /////////////////////////////////////////////////////////

//     methods->map_row_major(pixmap->pixels, free_array, NULL);  

//     free(cl);
//     Pnm_ppmfree(&pixmap);
}


Pnm_ppm create_ppm(Pnm_ppm src_image)
{
    assert(src_image != NULL);

    int width = src_image->width;
    int height = src_image->height;
    //int size = src_image->methods->size(src_image->pixels);
    int size = (int)sizeof(struct brightness);

    Pnm_ppm final_image = malloc(sizeof(*final_image));
    assert(final_image != NULL);
    
    final_image->width = width;
    final_image->height = height;
    final_image->denominator = src_image->denominator;
    final_image->methods = src_image->methods;
    final_image->pixels = final_image->methods->new(width, height, size); 

    return final_image;
}

void copy_array(int col, int row, A2Methods_UArray2 src_image,
                A2Methods_Object *rgb_elem, void *closure)
{
    (void)src_image;

    A2Methods_T methods = ((partial_pnm *)closure)->methods;
    A2Methods_UArray2 destination = ((partial_pnm *)closure)->aux_array;

    int width = methods->width(destination);
    int height = methods->height(destination);

    if (col < width && row < height){
        *((Pnm_rgb)methods->at(destination, col, row)) = *((Pnm_rgb)rgb_elem);
        Pnm_rgb dest = (Pnm_rgb)methods->at(destination, col, row);

        //printf("R: %u, G: %u, B: %u\n", (*dest)->red, (*dest)->green, (*dest)->blue);
        
    }

}

void RGB_to_VCS(int col, int row, A2Methods_UArray2 src_image,
                A2Methods_Object *rgb_elem, void *cl)
{
    (void)src_image;
    printf("\ncol: %d and row: %d\n", col, row);
    
    A2Methods_T methods = ((partial_pnm *)cl)->methods;
    A2Methods_UArray2 destination = ((partial_pnm *)cl)->aux_array;

    brightness *VCS_values = convert_RGB((Pnm_rgb)rgb_elem);
    brightness *dest = (methods)->at(destination, col, row);

    *dest = *VCS_values; //TODO: investigate

}

//IN PROGRESS
void VCS_to_RGB(int col, int row, A2Methods_UArray2 src_image,
                A2Methods_Object *vcs_elem, void *cl)
{
/* src_image holds brightness struct, cl/destination holds rgb struct */

    A2Methods_T methods = ((partial_pnm *)cl)->methods;
    A2Methods_UArray2 destination = ((partial_pnm *)cl)->aux_array;

    Pnm_rgb RGB_values = convert_VCS((brightness *)vcs_elem);
    printf("testing...\n");
    fprintf(stderr, "\ncol: %d and row: %d\n", col, row);
    fprintf(stderr, "r: %u, g: %u, b: %u\n", RGB_values->red, RGB_values->green, RGB_values->blue);

    Pnm_rgb *dest = methods->at(destination, col, row);
    // NOTE: THIS PRINT STATEMENT WAS CAUSING THE SEGFAULT!!!!!!!
    //fprintf(stderr, "r: %u, g: %u, b: %u\n", (*dest)->red, (*dest)->green, (*dest)->blue);

    *dest = RGB_values; 

}


brightness *convert_RGB(Pnm_rgb RGB)
{
    brightness *VCS_values = malloc(sizeof(struct brightness)); 
    VCS_values->Y =  (0.299 * RGB->red) + (0.587 * RGB->green) + (0.114 * RGB->blue);
    VCS_values->PB = (-0.168736 * RGB->red) - (0.331264 * RGB->green) + (0.5 * RGB->blue);
    VCS_values->PR = (0.5 * RGB->red) - (0.418688 * RGB->green) - (0.081312 * RGB->blue);
    printf("R: %u, G: %u, B: %u\n", RGB->red, RGB->green, RGB->blue);
    printf("Y: %f, PB: %f, PR: %f\n",  VCS_values->Y,  VCS_values->PB,  VCS_values->PR);
    return VCS_values;
}
//IN PROGRESS!
Pnm_rgb convert_VCS(brightness *VCS)
{
    Pnm_rgb RGB_values = malloc(sizeof(struct Pnm_rgb));
    RGB_values->red = 1.0 * VCS->Y + 0.0 * VCS->PB + 1.402 * VCS->PR;
    RGB_values->green = 1.0 * VCS->Y - 0.344136 * VCS->PB - 0.714136 * VCS->PR;
    RGB_values->blue = 1.0 * VCS->Y + 1.772 * VCS->PB + 0.0 * VCS->PR;
    
    printf("original R: %u, G: %u, B: %u\n", RGB_values->red, RGB_values->green, RGB_values->blue);
    //printf("Y: %f, PB: %f, PR: %f\n",  VCS_values->Y,  VCS_values->PB,  VCS_values->PR);
    return RGB_values;

    // TODO: Don't forget to free RGB values!
}

extern void decompress40(FILE *input) 
{
    unsigned height, width;
    int read = fscanf(input, "COMP40 Compressed image format 2\n%u %u",
                      &width, &height);
    assert(read == 2);
    int c = getc(input);
    assert(c == '\n');
    printf("width: %u, height: %u\n", width, height);
    // printf("hello\n");
    // Pnm_ppm pixmap = Pnm_ppmread(input);
    // assert(pixmap);
}

void free_array(int col, int row, A2Methods_UArray2 src_image,
                A2Methods_Object *vcs_elem, void *cl)
{
    // (void)col;
    // (void)row;
    (void)src_image;
    (void)cl;
    printf("freeing %f at %d, %d\n", ((brightness *)vcs_elem)->Y, col, row);
    //free((brightness *)vcs_elem);
}