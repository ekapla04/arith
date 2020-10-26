#include "decompression.h"

void unquantize_chroma(block_info block, Pnm_CVS CVS)
{
    // Pnm_CVS CVS = malloc(sizeof(struct Pnm_CVS));

    CVS.PB = Arith40_chroma_of_index(block.PB_4bit);
    CVS.PR = Arith40_chroma_of_index(block.PR_4bit);
    printf("RESTORED PB: %f, PR: %f\n", CVS.PB, CVS.PR);
}

void inverse_DCT(Pnm_CVS pixel1, Pnm_CVS pixel2, 
                 Pnm_CVS pixel3, Pnm_CVS pixel4, block_info block)
{

    float a = block.a;
    float b = block.b;
    float c = block.c;
    float d = block.d;

    pixel1.Y = a - b - c + d;
    pixel2.Y = a - b + c - d;
    pixel3.Y = a + b - c - d;
    pixel4.Y = a + b + c + d;

    printf("INVERT DCT Y1: %f | Y2: %f | Y3: %f | Y4: %f\n" , pixel1.Y, pixel2.Y,
                 pixel3.Y, pixel4.Y);
}

float unquantize_degree_brightness(int64_t degree)
{
    if (degree <= -15){
        return -0.3;

    } else if (degree >= -15 && degree <= -10){
        return -.25;

    } else if (degree >= -10 && degree <= -5){
        return -.15;

    } else if (degree >= -5 && degree <= 0){
        return -.05;

    } else if (degree >= 0 && degree <= 5){
        return .05;

    } else if (degree >= 5 && degree <= 10){
        return .15;

    } else if (degree >= 10 && degree <= 15){
        return .25;

    } else if (degree >= 15){
        return .3;

    }
    return 0;
}

