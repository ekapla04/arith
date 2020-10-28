/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*                           compress40.c                               *
*                                                                      * 
*       Assignment: arith                                              *
*       Authors: Emily Gjertsson (egjert01) & Elise Kaplan (ekapla04)  *  
*       Date: 10/26/2020                                               *
*                                                                      *
*       Summary                                                        *
*           Called from 40image.c, runs compression or decompression   *
*           on a high level                                            *
*                                                                      *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "assert.h"
#include "pnm.h"
#include "a2methods.h"
#include "a2plain.h"

#include "compress40.h"
#include "transformCVS.h"

/* main driver for image compression */
extern void compress40(FILE *input)
{
    assert(input != NULL);

    Pnm_ppm pixmap = read_pixmap(input);
    convert_to_CVS(pixmap);

    printf("COMP40 Compressed image format 2\n%u %u\n", pixmap->width, 
                                                        pixmap->height);
    compress_by_block(pixmap->pixels);

    Pnm_ppmfree(&pixmap);
}

/* main driver for image decompression */
extern void decompress40(FILE *input) 
{
    assert(input != NULL);
    Pnm_ppm pixmap = make_pixmap(input);
    A2Methods_UArray2 CVS_array = pixmap->methods->new(pixmap->width,
                                  pixmap->height, sizeof(Pnm_CVS));
    read_codeword(CVS_array, input);

    pixmap->methods->map_row_major(CVS_array, CVS_to_RGB, pixmap->pixels);

    Pnm_ppmwrite(stdout, pixmap);

    pixmap->methods->free(&CVS_array);
    Pnm_ppmfree(&pixmap);
}
