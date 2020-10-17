#include "compress40.h"
#include <stdlib.h>
#include <stdio.h>
#include "pnm.h"
#include "assert.h"
#include "a2methods.h"
#include "a2plain.h"
#include "stdbool.h"
#include "string.h"

extern void compress40  (FILE *input){
    // printf("heyo\n");
    A2Methods_T methods = uarray2_methods_plain; 
    assert(methods);
    Pnm_ppm pixmap = Pnm_ppmread(input, methods);
    assert(pixmap);
    if (pixmap->width % 2 != 0) {
        pixmap->width = pixmap->width - 1;
    }
    if (pixmap->height % 2 != 0) {
        pixmap->height = pixmap->height - 1;
    }
    Pnm_ppmfree(&pixmap);
}

extern void decompress40(FILE *input){
    // printf("hello\n");
    // Pnm_ppm pixmap = Pnm_ppmread(input);
    // assert(pixmap);
}