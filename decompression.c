/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
*                           decompression.c
*                               
*       Assignment: arith
*       Authors: Emily Gjertsson (egjert01) & Elise Kaplan (ekapla04)    
*       Date: 10/26/2020
*
*       Summary
*           implementation of decompression.h
*           Handles all decompression steps that occur block-by-block
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "decompression.h"

const int DENOMINATOR = 255;
const int BYTE = 8;
const int BLOCK = 2;

const long double BCD_MAX = 0.5;
const long double BCD_MIN = -0.5;
const long double Y_MAX = 1;
const long double Y_MIN = 0;

const int A_FACTOR = 511;
const long double SCALE_FACTOR = 50;


Pnm_ppm make_pixmap(FILE *input)
{
    assert(input != NULL);

    unsigned height, width;
    int read = fscanf(input, "COMP40 Compressed image format 2\n%u %u",
                      &width, &height);
    assert(read == 2);
    int c = getc(input);
    assert(c == '\n');

    Pnm_ppm pixmap = malloc(sizeof(struct Pnm_ppm));
    pixmap->denominator = DENOMINATOR;
    pixmap->width = width % 2 == 0 ? width : width - 1; 
    pixmap->height = height % 2 == 0 ? height : height - 1;
    pixmap->methods = uarray2_methods_plain; 
    assert(pixmap->methods != NULL);
    pixmap->pixels = pixmap->methods->new(pixmap->width, pixmap->height,
                                    sizeof(struct Pnm_rgb));

    assert(pixmap != NULL && pixmap->pixels != NULL);

    return pixmap;
}


void read_codeword(A2Methods_UArray2 CVS_array, FILE *input) 
{
    assert(CVS_array != NULL && input != NULL);

    uint64_t word = 0;
    A2Methods_T methods = uarray2_methods_plain;
    /* iterate across pixmap in row-major fashion */
    for (int col = 0; col < methods->height(CVS_array); col += 2) {
        for (int row = 0; row < methods->width(CVS_array); row += 2) {
            /* read codeword in 8 bit increments */
            for (int i = 24; i >= 0; i -= 8) {
                uint64_t value = fgetc(input);
                // assert((char)value != EOF);
                word = Bitpack_newu(word, 8, i, (uint64_t)value);
            }
            unpack_codeword(word, row, col, CVS_array);
        }
    }
}


void unpack_codeword(uint64_t word, int row, int col, 
                    A2Methods_UArray2 CVS_array)
{
    assert(CVS_array != NULL);
    block_info block; 

    block.a = Bitpack_getu(word, 9, 23);
    block.b = Bitpack_gets(word, 5, 18);
    block.c = Bitpack_gets(word, 5, 13);
    block.d = Bitpack_gets(word, 5, 8);
    block.PB_4bit = Bitpack_getu(word, 4, 4);
    block.PR_4bit = Bitpack_getu(word, 4, 0);


    Pnm_CVS pixel1, pixel2, pixel3, pixel4;

    unquantize_values(&pixel1, &pixel2, &pixel3, &pixel4, &block);    
    place_pixels(pixel1, pixel2, pixel3, pixel4, CVS_array, row, col);
    
}

void place_pixels(Pnm_CVS pixel1, Pnm_CVS pixel2, Pnm_CVS pixel3, 
                 Pnm_CVS pixel4, A2Methods_UArray2 CVS_array, int row, int col)
{
    assert(CVS_array != NULL);
    A2Methods_T methods = uarray2_methods_plain; 

    *((Pnm_CVS *)methods->at(CVS_array, row, col)) = pixel1;

    *((Pnm_CVS *)methods->at(CVS_array, row + 1, col)) = pixel2;

    *((Pnm_CVS *)methods->at(CVS_array, row, col + 1)) = pixel3;

    *((Pnm_CVS *)methods->at(CVS_array, row + 1, col + 1)) = pixel4;


}

void unquantize_values(Pnm_CVS *pixel1, Pnm_CVS *pixel2, 
                 Pnm_CVS *pixel3, Pnm_CVS *pixel4, block_info *block)
{
    assert(pixel1 != NULL && pixel2 != NULL && pixel3 != NULL && 
           pixel4 != NULL && block != NULL);

    *pixel1 = unquantize_chroma(block);
    *pixel2 = unquantize_chroma(block);
    *pixel3 = unquantize_chroma(block);
    *pixel4 = unquantize_chroma(block);

    inverse_DCT(pixel1, pixel2, pixel3, pixel4, block);

}


void inverse_DCT(Pnm_CVS *pixel1, Pnm_CVS *pixel2, 
                 Pnm_CVS *pixel3, Pnm_CVS *pixel4, block_info *block)
{
    assert(pixel1 != NULL && pixel2 != NULL && pixel3 != NULL && 
           pixel4 != NULL && block != NULL);

    long double a = (long double)(block->a) / 511.0;
    long double b = unquantize_degree_brightness(block->b);
    long double c = unquantize_degree_brightness(block->c);
    long double d = unquantize_degree_brightness(block->d);

    pixel1->Y = rounder((a - b - c + d), 0, 1);
    pixel2->Y = rounder((a - b + c - d), 0, 1);
    pixel3->Y = rounder((a - b + c - d), 0, 1);
    pixel4->Y = rounder((a + b + c + d), 0, 1);

}

Pnm_CVS unquantize_chroma(block_info *block)
{
    assert(block != NULL);
    struct Pnm_CVS CVS_values;
    CVS_values.PB = Arith40_chroma_of_index(block->PB_4bit);
    CVS_values.PR = Arith40_chroma_of_index(block->PR_4bit);
    return CVS_values;
}

long double unquantize_degree_brightness(int64_t degree)
{
    if (degree == 0) {
        return 0;
    }

    long double rounded = (long double)degree / 50.0;

    if(rounded < -0.3) {
        return -0.3;
    } else if (rounded > 0.3) {
        return 0.3;
    } 
    return rounded;
}

long double rounder(long double to_round, long double low, long double high)
{
    if (to_round < low) {
        to_round = low;
    } else if (to_round > high) {
        to_round = high;
    }
    return to_round;
}

