/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*                           compression.c                              *
*                                                                      * 
*       Assignment: arith                                              *
*       Authors: Emily Gjertsson (egjert01) & Elise Kaplan (ekapla04)  *  
*       Date: 10/26/2020                                               *
*                                                                      *
*       Summary                                                        *
*           implementation on compression.h                            *
*           Handles all compression that happens block-by-block        *
*           (average PB/PR, quantize Y values of block, make           *
*           them packables, pack into a single integer) and print      *
*           the bitpacked integer byte by byte                         *
*                                                                      *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "compression.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
    Functions in this file handle compression steps for 
    2x2 blocks
    Memory allocation : None
     C.R.Es : C.R.E if pointers are null                                                
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void compress_by_block(A2Methods_UArray2 CVS_array)
{
    assert(CVS_array != NULL);

    A2Methods_T methods = uarray2_methods_plain; 
    int width = methods->width(CVS_array);
    int height = methods->height(CVS_array);

    for (int col = 0; col < height; col += 2) {
        for (int row = 0; row < width; row += 2) {
            Pnm_CVS pixel1 = *((Pnm_CVS *)methods->at(CVS_array, 
                                                      row, col));

            Pnm_CVS pixel2 = *((Pnm_CVS *)methods->at(CVS_array, 
                                                      row + 1, col));

            Pnm_CVS pixel3 = *((Pnm_CVS *)methods->at(CVS_array, 
                                                      row, col + 1));

            Pnm_CVS pixel4 = *((Pnm_CVS *)methods->at(CVS_array, 
                                                      row + 1, col + 1));

            block_info block = quantize_chroma(pixel1, pixel2, pixel3, pixel4);

            perform_DCT(pixel1, pixel2, pixel3, pixel4, &block);

            make_codeword(block);

        }
    }
}

/* Steps 1 & 2 -- Average PB and PR, then make them packables */
block_info quantize_chroma(Pnm_CVS pixel1, Pnm_CVS pixel2, 
                           Pnm_CVS pixel3, Pnm_CVS pixel4)
{
    long double avg_PB = ((pixel1.PB + pixel2.PB + pixel3.PB + pixel4.PB) / 4);
    long double avg_PR = ((pixel1.PR + pixel2.PR + pixel3.PR + pixel4.PR) / 4);

    block_info block;
 
    block.PR_4bit = Arith40_index_of_chroma(avg_PR); 
    block.PB_4bit = Arith40_index_of_chroma(avg_PB);

    return block;
}

/* Steps 3 & 4 -- Quantize Y1, Y2, Y3, Y4 and make them packables*/
void perform_DCT(Pnm_CVS pixel1, Pnm_CVS pixel2, 
                 Pnm_CVS pixel3, Pnm_CVS pixel4, block_info *block)
{
    assert(block != NULL);

    long double Y1 = pixel1.Y;
    long double Y2 = pixel2.Y;
    long double Y3 = pixel3.Y; 
    long double Y4 = pixel4.Y;

    long double a = (Y4 + Y3 + Y2 + Y1) / 4.0;
    long double b = (Y4 + Y3 - Y2 - Y1) / 4.0;
    long double c = (Y4 - Y3 + Y2 - Y1) / 4.0;
    long double d = (Y4 - Y3 - Y2 + Y1) / 4.0;

    block->a = round(a * 511);
    block->b = quantize_degree_brightness(b);
    block->c = quantize_degree_brightness(c);
    block->d = quantize_degree_brightness(d);

}


int64_t quantize_degree_brightness(long double degree)
{
    int64_t rounded = round(degree * SCALE_FACTOR);
    
    if(rounded < -15) {
        return -15;
    } else if (rounded > 15) {
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
    print_word(word);
    
}

void print_word(uint64_t word)
{
    for (int i = 24; i >= 0; i -= 8) {
        putchar(Bitpack_getu(word, 8, i));
    }
}

