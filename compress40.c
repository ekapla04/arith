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

typedef struct closure {
    unsigned *denominator;
    char* operation;
} closure;


/* * * * * * * * * * Function declarations * * * * * * * * * * * */
void copy_array(int col, int row, A2Methods_UArray2 src_img,
                A2Methods_Object *rgb_elem, void *dest_array);


void RGB_to_VCS(int col, int row, A2Methods_UArray2 array2,
                A2Methods_Object *ptr, void *VCS_array);

brightness *convert_RGB(Pnm_rgb RGB);


void VCS_to_RGB(int col, int row, A2Methods_UArray2 src_img,
                A2Methods_Object *vcs_elem, void *cl);

Pnm_rgb convert_VCS(brightness *VCS);

void free_RGB_array(int col, int row, A2Methods_UArray2 src_img,
                A2Methods_Object *rgb_elem, void *cl);

void free_VCS_array(int col, int row, A2Methods_UArray2 src_img,
                A2Methods_Object *vcs_elem, void *cl);

// void standardize_RGB(int col, int row, A2Methods_UArray2 src_img,
//                 A2Methods_Object *rgb_elem, void *cl);

// void restore_RGB(int col, int row, A2Methods_UArray2 src_img,
//                 A2Methods_Object *rgb_elem, void *cl);


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
// Pnm_ppm create_ppm(Pnm_ppm src_img);

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

extern void compress40(FILE *input)
{
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
    

    methods->map_row_major(pixmap->pixels, copy_array, square_array);

    methods->free(&(pixmap->pixels));
    pixmap->pixels = square_array;

    // closure *cl = malloc(sizeof(struct closure));
    // cl->denominator =  &pixmap->denominator;
    // printf("denominator: %d", *(cl->denominator));
    // cl->operation = "*";


    // need to eventually free this memory
    // unsigned *denominator;
    // denominator = &pixmap->denominator;
    // printf("************** STANDARDIZING ORIGINAL RGB VALUES **************\n");
    // methods->map_row_major(pixmap->pixels, standardize_RGB, cl); 

    // cl->operation = "/";

    // methods->map_row_major(pixmap->pixels, standardize_RGB, cl);
    // printf("************** RESTORING ORIGINAL RGB VALUES ******************\n");
    // methods->map_row_major(pixmap->pixels, restore_RGB, denominator); 

    /* * * * * * * * * * * * convert to VCS * * * * * * * * * * */
    A2Methods_UArray2 VCS_array = methods->new(pixmap->width, pixmap->height,
                                        sizeof(struct brightness));

    //printf("R: %u, G: %u, B: %u\n", (*dest)->red, (*dest)->green, (*dest)->blue);

    methods->map_row_major(pixmap->pixels, RGB_to_VCS, VCS_array);
    methods->free(&(pixmap->pixels));
    pixmap->pixels = VCS_array;

    
    /* * * * * * * * * * * * * * REVERSION * * * * * * * * * * */

    printf("\n     ---STARTING REVERSION---\n");
    //ONLY GETS THROUGH ONE ROUND BEFORE SEGFAULT!
    A2Methods_UArray2 square_array2 = methods->new(pixmap->width, pixmap->height,
                                        sizeof(struct Pnm_rgb));

    methods->map_row_major(pixmap->pixels, VCS_to_RGB, square_array2); 
    methods->free(&(pixmap->pixels));
    pixmap->pixels = square_array2;

    printf("\nfinished reversion\n");

    // /////////////////////////////////////////////////////////
    // // methods->map_row_major(pixmap->pixels, free_RGB_array, NULL);  
    // // methods->map_row_major(pixmap->pixels, free_VCS_array, NULL);  
    Pnm_ppmfree(&pixmap);
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
/*

// void standardize_RGB(int col, int row, A2Methods_UArray2 src_img,
//                 A2Methods_Object *rgb_elem, void *cl)
// {
//     (void)col;
//     (void)row;
//     (void)src_img;

//     // what the fuck is this syntax :)
//     unsigned *denominator = cl->denominator;
//     float temp = (float)(*denominator);

//     char* operation = cl->operation;

//     A2Methods_T methods = uarray2_methods_plain; 

//     Pnm_rgb RGB = ((Pnm_rgb) rgb_elem);
//     if (strcmp(operation, "*")) {
//         RGB->red = ((float)RGB->red * temp);
//         RGB->green = ((float)RGB->green * temp);
//         RGB->blue = ((float)RGB->blue * temp);
//         printf("red_*: %d, blue_*: %d, green_*: %d\n", RGB->red, RGB->blue, RGB->green);
//     }
//     else if (strcmp(operation, "/")) {
//         RGB->red = ((float)RGB->red / temp);
//         RGB->green = ((float)RGB->green / temp);
//         RGB->blue = ((float)RGB->blue / temp);
//         printf("red/: %d, blue/: %d, green/: %d\n", RGB->red, RGB->blue, RGB->green);
//     }
//     // printf("red_: %d, blue_o: %d, green_o: %d\n", RGB->red, RGB->blue, RGB->green);

//     *((Pnm_rgb)methods->at(src_img, col, row)) = *RGB; 
//     // losing a lot of precision --> 7/15 becomes 0.....
// }

void copy_array(int col, int row, A2Methods_UArray2 src_img,
                A2Methods_Object *rgb_elem, void *dest_img)
{
    (void)src_img;

    A2Methods_T methods = uarray2_methods_plain; 
    dest_img = (A2Methods_UArray2)dest_img;

    int width = methods->width(dest_img);
    int height = methods->height(dest_img);

    if (col < width && row < height){
        *((Pnm_rgb)methods->at(dest_img, col, row)) = *((Pnm_rgb)rgb_elem);        
    }

}

/* alternative at: *((brightness *)methods->at(VCS_array, col, row)) = *((brightness *)VCS_values);
*/
void RGB_to_VCS(int col, int row, A2Methods_UArray2 src_img,
                A2Methods_Object *rgb_elem, void *VCS_array)
{
    (void)src_img;
    printf("\ncol: %d and row: %d\n", col, row);
    
    A2Methods_T methods = uarray2_methods_plain; 
    VCS_array = (A2Methods_UArray2)VCS_array;

    brightness *VCS_values = convert_RGB((Pnm_rgb)rgb_elem);

    brightness *dest = (methods)->at(VCS_array, col, row);
    *dest = *VCS_values; //TODO: investigate


}



brightness *convert_RGB(Pnm_rgb RGB)
{
    brightness *VCS_values = malloc(sizeof(struct brightness)); 
    float r = RGB->red;
    float b = RGB->blue;
    float g = RGB->green;
    VCS_values->Y =  (0.299 * r) + (0.587 * g) + (0.114 * b);

    VCS_values->PB = (-0.168736 * r) - (0.331264 * g) +  (0.5 * b);

    VCS_values->PR = (0.5 * r) - (0.418688 * g) - (0.081312 * b);

    if (r != 0 || g != 0 || b != 0){
        printf("R: %f, G: %f, B: %f\n", r, g, b);
        printf("Y: %f, PB: %f, PR: %f\n",  VCS_values->Y,  VCS_values->PB,  VCS_values->PR);
    }

    return VCS_values;
}

//IN PROGRESS
/* alternative at: *((Pnm_rgb)methods->at(RGB_array, col, row)) = *(Pnm_rgb)RGB_values;
*/
void VCS_to_RGB(int col, int row, A2Methods_UArray2 src_img,
                A2Methods_Object *vcs_elem, void *RGB_array)
{
/* src_img holds brightness struct, cl/destination holds rgb struct */
    (void)src_img;
    fprintf(stderr, "\ncol: %d and row: %d\n", col, row);

    A2Methods_T methods = uarray2_methods_plain; 
    RGB_array = (A2Methods_UArray2)RGB_array;

    Pnm_rgb RGB_values = convert_VCS((brightness *)vcs_elem);
    // fprintf(stderr, "r: %u, g: %u, b: %u\n", RGB_values->red, RGB_values->green, RGB_values->blue);

    Pnm_rgb *dest = methods->at(RGB_array, col, row);
    // NOTE: THIS PRINT STATEMENT WAS CAUSING THE SEGFAULT!!!!!!!
    //fprintf(stderr, "r: %u, g: %u, b: %u\n", (*dest)->red, (*dest)->green, (*dest)->blue);
    *dest = RGB_values; 
    
    //fprintf(stderr, "r: %u, g: %u, b: %u\n", (*dest)->red, (*dest)->green, (*dest)->blue);

}

//IN PROGRESS!
Pnm_rgb convert_VCS(brightness *VCS)
{
    Pnm_rgb RGB_values = malloc(sizeof(struct Pnm_rgb));

    RGB_values->red = (1.0 * VCS->Y) + (0.0 * VCS->PB) + (1.402 * VCS->PR);
    RGB_values->green = (1.0 * VCS->Y) - (0.344136 * VCS->PB) - 
                                (0.714136 * VCS->PR);
    RGB_values->blue = (1.0 * VCS->Y) + (1.772 * VCS->PB) + (0.0 * VCS->PR);
    
    if (RGB_values->red != 0 || RGB_values->green != 0 || RGB_values->blue != 0){
            printf("original R: %u, G: %u, B: %u\n", RGB_values->red, RGB_values->green, RGB_values->blue);
    }

    //printf("Y: %f, PB: %f, PR: %f\n",  VCS_values->Y,  VCS_values->PB,  VCS_values->PR);
    return RGB_values;

    // TODO: Don't forget to free RGB values!
}


void free_VCS_array(int col, int row, A2Methods_UArray2 src_img,
                A2Methods_Object *vcs_elem, void *cl)
{
    // (void)col;
    // (void)row;
    (void)src_img;
    (void)cl;
    printf("freeing %f at %d, %d\n", ((brightness *)vcs_elem)->Y, col, row);
    free((brightness *)vcs_elem);
}

void free_RGB_array(int col, int row, A2Methods_UArray2 src_img,
                A2Methods_Object *rgb_elem, void *cl)
{
    // (void)col;
    // (void)row;
    (void)src_img;
    (void)cl;
    printf("freeing %u at %d, %d\n", ((Pnm_rgb)rgb_elem)->red, col, row);
    free((Pnm_rgb)rgb_elem);
}



//////////////////////////////////////////////////////

// Pnm_ppm create_ppm(Pnm_ppm src_img)
// {
//     assert(src_img != NULL);

//     int width = src_img->width;
//     int height = src_img->height;
//     //int size = src_img->methods->size(src_img->pixels);
//     int size = (int)sizeof(struct brightness);

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
