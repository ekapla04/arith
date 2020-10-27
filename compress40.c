/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
*                           compress40.c
*                               
*       Assignment: arith
*       Authors: Emily Gjertsson (egjert01) & Elise Kaplan (ekapla04)    
*       Date: 10/26/2020
*
*       Summary
*           Called from 40image.c, runs compression or decompression
*           on a high level
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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

const int DENOMINATOR = 255;
const int BYTE = 8;
const int BLOCK = 2;

const float BCD_MAX = 0.5;
const float BCD_MIN = -0.5;
const float Y_MAX = 1;
const float Y_MIN = 0;

const int A_FACTOR = 511;
const float SCALE_FACTOR = 50;

extern void decompress40(FILE *input) 
{
    unsigned height, width;
    int read = fscanf(input, "COMP40 Compressed image format 2\n%u %u",
                      &width, &height);
    assert(read == 2);
    int c = getc(input);
    assert(c == '\n');

    //printf("width: %u, height: %u\n", width, height);

    Pnm_ppm pixmap = malloc(sizeof(struct Pnm_ppm));
    pixmap->denominator = DENOMINATOR;
    pixmap->width = width % 2 == 0 ? width : width - 1; //CHECK IF WIDTH IS EVEN
    pixmap->height = height % 2 == 0 ? height : height - 1; // CHECK IF HEIGHT IS EVEN
    pixmap->methods = uarray2_methods_plain; 
    assert(pixmap->methods);
    pixmap->pixels = pixmap->methods->new(pixmap->width, pixmap->height,
                                    sizeof(struct Pnm_rgb));

    assert(pixmap && pixmap->pixels);

    //TODO: halligan reads in little-endian?

   A2Methods_UArray2 CVS_array = pixmap->methods->new(pixmap->width, pixmap->height,
                                        sizeof(struct Pnm_CVS));
    uint64_t word = 0;
    for (unsigned col = 0; col < pixmap->height; col += 2) {
        for (unsigned row = 0; row < pixmap->width; row += 2) {
           // printf("row %d, col: %d\n", row, col);

            for (int i = 24; i >= 0; i -= 8) {
                uint64_t value = fgetc(input);
                //assert(value != EOF);
                //printf("value in c40: %lu, hex: 0x%lx\n", value, value);
                word = Bitpack_newu(word, 8, i, (uint64_t)value);

            }
            unpack_codeword(word, row, col, CVS_array);

        }
    }

    Pnm_CVS spot = *((Pnm_CVS *)pixmap->methods->at(CVS_array, 3, 3));
    // printf("CVS values - Y: %f, PB: %f, PR: %f \n", spot.Y, spot.PB, spot.PR);

    // printf("   ----- CVS to RGB -----\n");
    pixmap->methods->map_row_major(CVS_array, CVS_to_RGB, pixmap->pixels);
    Pnm_ppmwrite(stdout, pixmap);

    pixmap->methods->free(&CVS_array);
    Pnm_ppmfree(&pixmap);

}

extern void compress40(FILE *input)
{
    //printf("\n     ---STARTING COMPRESSION---\n");

    Pnm_ppm pixmap = read_pixmap(input);
    convert_to_CVS(pixmap);


    /* * * * * * * * * * * * * * compress by block * * * * * * * * * * */
    printf("COMP40 Compressed image format 2\n%u %u\n", pixmap->width, pixmap->height);
    compress_by_block(pixmap->pixels);

    Pnm_ppmfree(&pixmap);
}