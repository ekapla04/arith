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


/* * * * * * * * private function declarations * * * * * * * * */
block_info quantize_chroma(Pnm_CVS pixel1, Pnm_CVS pixel2, 
                           Pnm_CVS pixel3, Pnm_CVS pixel4);

void perform_DCT(Pnm_CVS pixel1, Pnm_CVS pixel2, 
                       Pnm_CVS pixel3, Pnm_CVS pixel4, block_info *block);

int64_t quantize_degree_brightness(long double degree);

void make_codeword(block_info block);

void print_word(uint64_t word);
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* Driver function for block-by-block compression */
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

/*
* Purpose: Average PB and PR, then make them packables (4bit quantized values)
*           and store them in a new block_info struct
* Arguments: four pixels (Pnm_CVS structs)
* Returns: block_info struct
* Mem alloc: none
* C.R.E.: none
*/
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


/*
* Purpose: Quantize Y1, Y2, Y3, Y4 and make them packables (5bit values)
* Arguments: four pixels (Pnm_CVS structs) and block_info struct, passed by 
*            reference
* Returns: None (void)
* Mem alloc: none
* C.R.E.: if block_info struct is NULL
*/
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

    block->a = round(a * A_FACTOR);
    block->b = quantize_degree_brightness(b);
    block->c = quantize_degree_brightness(c);
    block->d = quantize_degree_brightness(d);

}

/* perform_DCT helper function, map values from (-0.3, 0.3) to (-15, 15) */
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
/*
* Purpose: Pack int values into one unsigned integer and call printing function
* Arguments: block_info struct
* Returns: None (void)
* Mem alloc: none
* C.R.E.: none
* NOTE: calls printing function to print the bitpacked unsigned integer
*/
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

/*
* Purpose: print bitpacked unsigned integer byte-by-byte
* Arguments: bitpacked unsigned integer
* Returns: None (void)
* Mem alloc: none
* C.R.E.: none
* NOTE: prints to standard out
*/
void print_word(uint64_t word)
{
    for (int i = 24; i >= 0; i -= 8) {
        putchar(Bitpack_getu(word, 8, i));
    }
}

