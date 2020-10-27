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

// take row and col of start of block
// take word
// unpack the word
float rounder(float to_round, float low, float high);


void unpack_codeword(uint64_t word, int row, int col, 
                    A2Methods_UArray2 CVS_array)
{
    block_info block; 

    block.a = Bitpack_getu(word, 9, 23);
    block.b = Bitpack_gets(word, 5, 18);
    block.c = Bitpack_gets(word, 5, 13);
    block.d = Bitpack_gets(word, 5, 8);
    block.PB_4bit = Bitpack_getu(word, 4, 4);
    block.PR_4bit = Bitpack_getu(word, 4, 0);

    // printf("a: %lu, b: %ld, c: %ld, d: %ld, PB: %lu, PR: %lu\n", block.a, block.b,
    //              block.c, block.d, block.PB_4bit, block.PR_4bit);


    Pnm_CVS pixel1, pixel2, pixel3, pixel4;

    unquantize_values(&pixel1, &pixel2, &pixel3, &pixel4, &block);    
    place_pixels(pixel1, pixel2, pixel3, pixel4, CVS_array, row, col);
    
    //use given function to convert four bit chroma back to averages (float Arith40_chroma_of_index(unsigned n))
    //Inverse DCT gives us Y1, Y2, Y3, and Y4, and now we have a VCS
}

void place_pixels(Pnm_CVS pixel1, Pnm_CVS pixel2, Pnm_CVS pixel3, 
                Pnm_CVS pixel4, A2Methods_UArray2 CVS_array, int row, int col)
{
    A2Methods_T methods = uarray2_methods_plain; 

    *((Pnm_CVS *)methods->at(CVS_array, row, col)) = pixel1;
    // printPixel(*((Pnm_CVS *)methods->at(CVS_array, row, col)), row, col);

    *((Pnm_CVS *)methods->at(CVS_array, row + 1, col)) = pixel2;
    // printPixel(*((Pnm_CVS *)methods->at(CVS_array, row+1, col)), row+1, col);

    *((Pnm_CVS *)methods->at(CVS_array, row, col + 1)) = pixel3;
    // printPixel(*((Pnm_CVS *)methods->at(CVS_array, row, col+1)), row, col+1);

    *((Pnm_CVS *)methods->at(CVS_array, row + 1, col + 1)) = pixel4;
    // printPixel(*((Pnm_CVS *)methods->at(CVS_array, row +1, col+1)), row+1, col+1);


}

void unquantize_values(Pnm_CVS *pixel1, Pnm_CVS *pixel2, 
                 Pnm_CVS *pixel3, Pnm_CVS *pixel4, block_info *block)
{
    *pixel1 = unquantize_chroma(block);
    *pixel2 = unquantize_chroma(block);
    *pixel3 = unquantize_chroma(block);
    *pixel4 = unquantize_chroma(block);

    //printf("pb: %f, pr: %f\n", pixel1->PB, pixel1->PR);
    inverse_DCT(pixel1, pixel2, pixel3, pixel4, block);

}


void inverse_DCT(Pnm_CVS *pixel1, Pnm_CVS *pixel2, 
                 Pnm_CVS *pixel3, Pnm_CVS *pixel4, block_info *block)
{

    // float a = block->a;
    // float b = block->b;
    // float c = block->c;
    // float d = block->d;
    float a = (float)(block->a) / 511.0;
    float b = unquantize_degree_brightness(block->b);
    float c = unquantize_degree_brightness(block->c);
    float d = unquantize_degree_brightness(block->d);
    // printf(" TEST -- a: %f, b: %f, c: %f, d: %f\n", a, b, c, d);

    pixel1->Y = rounder((a - b - c + d), 0, 1);
    pixel2->Y = rounder((a - b + c - d), 0, 1);
    pixel3->Y = rounder((a - b + c - d), 0, 1);
    pixel4->Y = rounder((a + b + c + d), 0, 1);

    // pixel1->Y = a - b - c + d;
    // pixel2->Y = a - b + c - d;
    // pixel3->Y = a + b - c - d;
    // pixel4->Y = a + b + c + d;

    // printf("INVERT DCT Y1: %f | Y2: %f | Y3: %f | Y4: %f\n" , pixel1->Y, pixel2->Y,
    //              pixel3->Y, pixel4->Y);
}

Pnm_CVS unquantize_chroma(block_info *block)
{
    struct Pnm_CVS CVS_values;
    CVS_values.PB = Arith40_chroma_of_index(block->PB_4bit);
    CVS_values.PR = Arith40_chroma_of_index(block->PR_4bit);
    return CVS_values;
}

float unquantize_degree_brightness(int64_t degree)
{
    if (degree == 0){
        return 0;
    }

    float rounded = (float)degree / 50.0;

    if(rounded < -0.3){
        return -0.3;
    } else if (rounded > 0.3){
        return 0.3;
    } 
    return rounded;
}

float rounder(float to_round, float low, float high)
{
    if (to_round < low){
        to_round = low;
    } else if (to_round > high){
        to_round = high;
    }

    return to_round;
}

void printPixel(Pnm_CVS CVS, int row, int col)
{
    printf("(%d, %d) - Y: %f, PB: %f, PR: %f\n", row, col, CVS.Y, CVS.PB, CVS.PR);
}