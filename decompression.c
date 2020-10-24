#include "decompression.h"

void unquantize_chroma(block_info block, Pnm_VCS VCS)
{
    // Pnm_VCS VCS = malloc(sizeof(struct Pnm_VCS));

    VCS->PB = Arith40_chroma_of_index(block->PB_4bit);
    VCS->PR = Arith40_chroma_of_index(block->PR_4bit);
    printf("RESTORED PB: %f, PR: %f\n", VCS->PB, VCS->PR);
}

void inverse_DCT(Pnm_VCS pixel1, Pnm_VCS pixel2, 
                 Pnm_VCS pixel3, Pnm_VCS pixel4, block_info block)
{

    float a = block->a;
    float b = block->b;
    float c = block->c;
    float d = block->d;

    pixel1->Y = a - b - c + d;
    pixel2->Y = a - b + c - d;
    pixel3->Y = a + b - c - d;
    pixel4->Y = a + b + c + d;

    printf("INVERT DCT p1: %f | p2: %f | p3: %f | p4: %f\n" , pixel1->Y, pixel2->Y,
                 pixel3->Y, pixel4->Y);

}

