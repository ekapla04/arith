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

typedef struct block_info {
    UArray_T *coordinates;
    uint64_t PR_4bit;
    uint64_t PB_4bit;
    uint64_t a;
    int64_t b;
    int64_t c;
    int64_t d;
} *block_info;


void compress_by_block(A2Methods_UArray2 VCS_array){
    A2Methods_T methods = uarray2_methods_plain; 
    int width = methods->width(VCS_array);
    int height = methods->height(VCS_array);
    printf("width: %d, height: %d\n", width, height);

    for (int row = 0; row < height; row += 2) {
        for (int col = 0; col < width; col += 2){
            printf("(%d, %d)\n", row, col);
            //average 4 values
            //quantize
            //stick em in structs
            //put struct in new array

            uint64_t PR_4bit = quantize_chroma(VCS_array);

        }
    }
}

uint64_t quantize_chroma(A2Methods_UArray2 VCS_array){

}
