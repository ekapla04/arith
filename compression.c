/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
*                           compression.c
*                               
*       Assignment: arith
*       Authors: Emily Gjertsson (egjert01) & Elise Kaplan (ekapla04)    
*       Date: 10/26/2020
*
*       Summary
*           implementation on compression.h
*           Handles all compression that happens block-by-block
*           (average PB/PR, quantize Y values of block, make
*           them packables, pack into a single integer) and print
*           the bitpacked integer byte by byte
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// #include <stdlib.h>
// #include <stdio.h>
// #include <stdbool.h>
// #include <string.h>
// #include <stdint.h>
// #include <math.h>

// #include "assert.h"

// #include "a2methods.h"
// #include "a2plain.h"
// #include "pnm.h"

// #include "compress40.h"
#include "compression.h"

void print_word(uint64_t word);


void compress_by_block(A2Methods_UArray2 CVS_array)
{
    A2Methods_T methods = uarray2_methods_plain; 
    int width = methods->width(CVS_array);
    int height = methods->height(CVS_array);
    //printf("width: %d, height: %d\n", width, height);
    // TODO: ENSURE ROW-MAJOR!!!!!!!!!! ERROR CHECK
    for (int col = 0; col < height; col += 2) {
        for (int row = 0; row < width; row += 2){
            //printf("(%d, %d)\n", row, col);

            //pixels in row-major order
            Pnm_CVS pixel1 = *((Pnm_CVS *)methods->at(CVS_array, row, col));
            //printf("(%d, %d) - Y: %f, PB: %f, PR: %f\n", row, col, pixel1.Y, pixel1.PB, pixel1.PR);

            Pnm_CVS pixel2 = *((Pnm_CVS *)methods->at(CVS_array, row + 1, col));
            // printf("(%d, %d) - Y: %f, PB: %f, PR: %f\n", row + 1, col, pixel2.Y, pixel2.PB, pixel2.PR);

            Pnm_CVS pixel3 = *((Pnm_CVS *)methods->at(CVS_array, row, col + 1));
            // printf("(%d, %d) - Y: %f, PB: %f, PR: %f\n", row, col + 1, pixel3.Y, pixel3.PB, pixel3.PR);

            Pnm_CVS pixel4 = *((Pnm_CVS *)methods->at(CVS_array, row + 1, col + 1));
            // printf("(%d, %d) - Y: %f, PB: %f, PR: %f\n", row + 1, col + 1, pixel4.Y, pixel4.PB, pixel4.PR);

            block_info block = quantize_chroma(pixel1, pixel2, pixel3, pixel4);

            // Pnm_CVS CVS = malloc(sizeof(struct Pnm_CVS)); // TODO: FREEE

            perform_DCT(pixel1, pixel2, pixel3, pixel4, &block);

            make_codeword(block);

            // inverse_DCT(pixel1, pixel2, pixel3, pixel4, block);

            // unquantize_chroma(block, CVS); // TODO: FREE BLOCK_INFO STRUCT

            //printf("\n\n");

        }
    }
}

/* Steps 1 & 2 -- Average PB and PR, then make them packables */
block_info quantize_chroma(Pnm_CVS pixel1, Pnm_CVS pixel2, 
                           Pnm_CVS pixel3, Pnm_CVS pixel4)
{

    float avg_PB = ((pixel1.PB + pixel2.PB + pixel3.PB + pixel4.PB)/4);
    float avg_PR = ((pixel1.PR + pixel2.PR + pixel3.PR + pixel4.PR)/4);
    // printf("AVG PB: %f, PR: %f\n", avg_PB, avg_PR);

    block_info block;
    
    // NEED TO DELETE SPACE FOR THIS 
    block.PR_4bit = Arith40_index_of_chroma(avg_PR);
    block.PB_4bit = Arith40_index_of_chroma(avg_PB);

   // printf("QUANT PR: %ld, PB: %ld\n", block.PR_4bit, block.PB_4bit);

    
    // printf("(p1: %f, %f | p2: %f, %f | p3: %f, %f | p4: %f, %f)\n"
    //          , pixel1.Y, pixel1.PB, pixel2.Y, pixel2.PB, 
    //         pixel3.Y, pixel3.PB, pixel4.Y, pixel4.PB);

    return block;
}

/* Steps 3 & 4 -- Quantize Y1, Y2, Y3, Y4 and make them packables*/
void perform_DCT(Pnm_CVS pixel1, Pnm_CVS pixel2, 
                 Pnm_CVS pixel3, Pnm_CVS pixel4, block_info *block)
{
    float Y1 = pixel1.Y;
    float Y2 = pixel2.Y;
    float Y3 = pixel3.Y; 
    float Y4 = pixel4.Y;
    // printf("(PRE DCT Y1: %f | Y2: %f | Y3: %f | Y4: %f)\n" , pixel1.Y, pixel2.Y,
    //          pixel3.Y, pixel4.Y);

    float a = (Y4 + Y3 + Y2 + Y1)/4.0;
    float b = (Y4 + Y3 - Y2 - Y1)/4.0;
    float c = (Y4 - Y3 + Y2 - Y1)/4.0;
    float d = (Y4 - Y3 - Y2 + Y1)/4.0;


    //printf(" BEFORE quant: a: %f, b: %f, c: %f, d: %f\n", a, b, c, d);

    block->a = (a * 511); //Question: round?
    block->b = quantize_degree_brightness(b);
    block->c = quantize_degree_brightness(c);
    block->d = quantize_degree_brightness(d);

    // printf("AFTER quant: a: %lu, b: %ld, c: %ld, d: %ld\n", block->a, block->b, block->c, block->d);

    // a = (float)block.a/511;
    // b = unquantize_degree_brightness(block.b);
    // c = unquantize_degree_brightness(block.c);
    // d = unquantize_degree_brightness(block.d);

    // printf(" UN quant: a: %f, b: %f, c: %f, d: %f\n", a, b, c, d);

}

int64_t quantize_degree_brightness(float degree)
{
    int rounded = round(degree * SCALE_FACTOR);
    
    if(rounded < -15){
        return -15;
    } else if (rounded > 15){
        return 15;
    } 

    return rounded;
}

/* Steps 5 & 6 -- pack into code words and print */
void make_codeword(block_info block)
{
    uint64_t word = 0;
    if (Bitpack_fitsu(block.a, 9)) {
        word = Bitpack_newu(word, 9, 23, block.a);
    }
    if (Bitpack_fitss(block.b, 5)) {
        word = Bitpack_news(word, 5, 18, block.b);
    }
    if (Bitpack_fitss(block.c, 5)) {
        word = Bitpack_news(word, 5, 13, block.c);
    }
    if (Bitpack_fitss(block.d, 5)) {
        word = Bitpack_news(word, 5, 8, block.d);
    }
    if (Bitpack_fitsu(block.PB_4bit, 4)) {
        word = Bitpack_newu(word, 4, 4, block.PB_4bit);
    }
    if (Bitpack_fitsu(block.PR_4bit, 4)) {
        word = Bitpack_newu(word, 4, 0, block.PR_4bit);
    }
    // printf("my number is 0x%04x\n",word);
    //printf("word (hex): %lx, word(dec): %lu\n", word, word);
    print_word(word);
    // printf("\n");
    
}

/* TODO: not working... must print out words byte-by-byte*/
/* 
WORD 1: 
001000100 00111 00011 0001 1100 0111
   a: 68  b: 7  c: 3  d: 3 PB:12 PR: 7

00100010 00011100 01100011 11000111
Byte 1: 34 ("), Byte 2: 28 (file separator), Byte 3: 99 (c), Byte 4: 199 (A w/~) --> extended

WORD 2:
000011111 11101 00011 11101 1010 1010
  a: 31   b:-3  c: 3  d:-3  PR:10 PB:10

00001111 11110100 01111101 10101010
Byte1: 15(f), Byte2: 244(carriage return i think), Byte3: 125(}), Byte4: 170 (weird r-like symbol)

*/
void print_word(uint64_t word)
{
    //BIG ENDIAN ORDER, most significant byte first
    for (int i = 24; i >= 0; i -= 8){
        //printf("i: %d -- ", i);
      //printf("value (hex): 0x%hhx", (char)Bitpack_getu(word, 8, i));
        //printf("value (char): %c", (char)Bitpack_getu(word, 8, i));
        putchar(Bitpack_getu(word, 8, i));
        //printf("\n");
    }
}

