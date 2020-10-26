#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "assert.h"
#include "pnm.h"
#include "a2methods.h"
#include "a2plain.h"

#include "compress40.h"
#include "transformCVS.h"

extern void decompress40(FILE *input) 
{
    // unsigned height, width;
    // int read = fscanf(input, "COMP40 Compressed image format 2\n%u %u",
    //                   &width, &height);
    // assert(read == 2);
    // int c = getc(input);
    // assert(c == '\n');


    // printf("width: %u, height: %u\n", width, height);
    // // A2Methods_UArray2 pixels = methods->new(pixmap->width, pixmap->height,
    // //                                 sizeof(struct Pnm_rgb));


    // Pnm_ppm pixmap = malloc(sizeof(struct Pnm_ppm));
    // pixmap->denominator = 255;
    // pixmap->width = width % 2 == 0 ? width : width - 1; //CHECK IF WIDTH IS EVEN
    // pixmap->height = height % 2 == 0 ? height : height - 1; // CHECK IF HEIGHT IS EVEN
    // pixmap->methods = uarray2_methods_plain; 
    // pixmap->pixels = pixmap->methods->new(pixmap->width, pixmap->height,
    //                                 sizeof(struct Pnm_rgb));

    // assert(pixmap && pixmap->pixels);

    // uint64 word = fgetc(input);

    // while (feof(input) == 0 && ferror(input) == 0){
    //     curr_byte = fgetc(input);
    //     assert(curr_byte != EOF);
    //     codeword = Bitpack_newu(codeword, 4, i, curr_byte);
    // }

    // /* read in line, char by char, until endline character */
    // while (feof(inputfd) == 0 && ferror(inputfd) == 0) {
    //     if (c == '\n') {
    //         *(*datapp + index) = c;
    //         break; /* line is finished reading, add \0 then return!*/
    //     }
    //     if (index + 2 == bufferSize) {
    //         *datapp = expand(*datapp, &bufferSize);
    //     }
    //     *(*datapp + index) = c;
    //     index++;
    //     c = fgetc(inputfd);
    // }


        /* * * * * * * * * * * * * * REVERSION * * * * * * * * * * */

    // printf("\n     ---STARTING REVERSION---\n");
    // //NOTE: only for testing, will move to decompression function, and get 
    // //       width and height from header
    // A2Methods_UArray2 square_array2 = methods->new(pixmap->width, pixmap->height,
    //                                     sizeof(struct Pnm_rgb));

    // methods->map_row_major(pixmap->pixels, CVS_to_RGB, square_array2); 
    // methods->free(&(pixmap->pixels));
    // pixmap->pixels = square_array2;
    //printf("sq array dimensions - width: %d, height: %d\n", pixmap->width, pixmap->height);



}

extern void compress40(FILE *input)
{
    //printf("\n     ---STARTING COMPRESSION---\n");

    Pnm_ppm pixmap = read_pixmap(input);
    convert_to_CVS(pixmap);
    
    //Pnm_ppmwrite(stdout, pixmap); --> CAN't tell if it works from this line

    /* * * * * * * * * * * * * * compress by block * * * * * * * * * * */
    printf("COMP40 Compressed image format 2\n%u %u\n", pixmap->width, pixmap->height);
    compress_by_block(pixmap->pixels);

    Pnm_ppmfree(&pixmap);
}