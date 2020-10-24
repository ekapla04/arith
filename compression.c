#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#include "assert.h"

#include "a2methods.h"
#include "a2plain.h"
#include "pnm.h"

#include "compress40.h"
#include "compression.h"


void compress_by_block(A2Methods_UArray2 VCS_array)
{
    A2Methods_T methods = uarray2_methods_plain; 
    int width = methods->width(VCS_array);
    int height = methods->height(VCS_array);
    printf("width: %d, height: %d\n", width, height);

    // X 0 X 0
    // 0 0 0 0
    // X 0 X 0
    // 0 0 0 0

    for (int row = 0; row < height; row += 2) {
        for (int col = 0; col < width; col += 2){
            printf("(%d, %d)\n", row, col);

            Pnm_VCS pixel1 = methods->at(VCS_array, col, row);
            Pnm_VCS pixel2 = methods->at(VCS_array, col, row + 1);
            Pnm_VCS pixel3 = methods->at(VCS_array, col + 1, row);
            Pnm_VCS pixel4 = methods->at(VCS_array, col + 1, row + 1);


            // lets make brightness ptrs to pixels at each location we want
            // maybe pass 4 pixels into quantize_chroma?
            // brightness p1, p2, p3, p4 = methods->at(jwppk);
                // ex. quantize_chroma()
            // average 4 values
            // quantize
            // stick em in structs
            // put struct in new array

            block_info block = quantize_chroma(pixel1, pixel2, pixel3, pixel4);

            Pnm_VCS VCS = malloc(sizeof(struct Pnm_VCS));

            perform_DCT(pixel1, pixel2, pixel3, pixel4, block);
            inverse_DCT(pixel1, pixel2, pixel3, pixel4, block);

            unquantize_chroma(block, VCS);
        }
    }
}

block_info quantize_chroma(Pnm_VCS pixel1, Pnm_VCS pixel2, 
                           Pnm_VCS pixel3, Pnm_VCS pixel4)
{

    float avg_PB = ((pixel1->PB + pixel2->PB + pixel3->PB + pixel4->PB)/4);
    float avg_PR = ((pixel1->PR + pixel2->PR + pixel3->PR + pixel4->PR)/4);
    printf("AVG PR: %f, PB: %f\n", avg_PR, avg_PB);

    block_info block = malloc(sizeof(struct block_info)); 
    // NEED TO DELETE SPACE FOR THIS 
    block->PR_4bit = Arith40_index_of_chroma(avg_PR);
    block->PB_4bit = Arith40_index_of_chroma(avg_PB);
    // printf("QUANT PR: %ld, PB: %ld\n", block->PR_4bit, block->PB_4bit);

    
    // printf("(p1: %f, %f | p2: %f, %f | p3: %f, %f | p4: %f, %f)\n"
    //          , pixel1->Y, pixel1->PB, pixel2->Y, pixel2->PB, 
    //         pixel3->Y, pixel3->PB, pixel4->Y, pixel4->PB);

    return block;
}

void perform_DCT(Pnm_VCS pixel1, Pnm_VCS pixel2, 
                 Pnm_VCS pixel3, Pnm_VCS pixel4, block_info block)
{
    float Y1 = pixel1->Y;
    float Y2 = pixel2->Y;
    float Y3 = pixel3->Y;
    float Y4 = pixel4->Y;
    printf("(PRE DCT p1: %f | p2: %f | p3: %f | p4: %f)\n" , pixel1->Y, pixel2->Y,
                 pixel3->Y, pixel4->Y);

    float a = (Y4 + Y3 + Y2 + Y1)/4.0;
    float b = (Y4 + Y3 - Y2 - Y1)/4.0;
    float c = (Y4 - Y3 + Y2 - Y1)/4.0;
    float d = (Y4 - Y3 - Y2 + Y1)/4.0;

    block->a = a;
    block->b = b;
    block->c = c;
    block->d = d;

    // printf("a: %f, b: %f, c: %f, d: %f\n", block->a, block->b, block->c, block->d);

}