/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*                           compression.h                              *
*                                                                      * 
*       Assignment: arith                                              *
*       Authors: Emily Gjertsson (egjert01) & Elise Kaplan (ekapla04)  *  
*       Date: 10/26/2020                                               *
*                                                                      *
*       Summary                                                        *
*           interface of compression.c                                 *
*           Handles compression block-by-block                         *
*                                                                      *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "assert.h"
#include "pnm.h"
#include "a2methods.h"
#include "a2plain.h"
#include "arith40.h"
#include "bitpack.h"
#include <math.h>

#include "decompression.h"
#include "compress40.h"

extern const long double SCALE_FACTOR;

/* Purpose : Print bitpacked word in 8 bit increments
 * Arguments : uint64_t that holds bitpacked word
 * Returns : Nothing
 * Notes : Uses putchar helper function to access parts of word
 * Memory allocation : None
 * C.R.Es : None
 */
void print_word(uint64_t word);

/* Purpose : Driver function for 2x2 block compression, 
 *           gets pixels of 2x2 block
 * Arguments : UArray2_T
 * Returns : A Pnm_ppm struct containing the initial image
 * Notes : Assert that the Pnm_ppm and methods is not NULL
 * Memory allocation : Memory allocated for Pnm_ppm, A2Methods_UArray2,
 *                     A2Methods_T, and Pnm_rgb struct
 * C.R.Es : C.R.E if image is not in correct format or Pnm_ppm, methods,  
 *          or input are null
 */
void compress_by_block(A2Methods_UArray2 CVS_array);

/* Purpose : Average PB and PR values of 2x2 and quantize them
 * Arguments : 4 pixels in a 2x2 block
 * Returns : Block info struct
 * Notes : Uses helper function Artih40_index_of_chroma
 * Memory allocation : None
 * C.R.Es : None
 */
block_info quantize_chroma(Pnm_CVS pixel1, Pnm_CVS pixel2, 
                           Pnm_CVS pixel3, Pnm_CVS pixel4);


/* Purpose : Converts Y values to cosine brightness values
 * Arguments : 4 pixels in a 2x2 blocks
 * Returns : Nothing
 * Notes : Assert that the block is not NULL
 * Memory allocation : None
 * C.R.Es : None
 */
void perform_DCT(Pnm_CVS pixel1, Pnm_CVS pixel2, 
                       Pnm_CVS pixel3, Pnm_CVS pixel4, block_info *block);

/* Purpose : Quantizes cosine brightness values between -15 and 15
 * Arguments : cosine degree
 * Returns : int64_t
 * Notes : None
 * Memory allocation : None
 * C.R.Es : None
 */
int64_t quantize_degree_brightness(long double degree);

/* Purpose : Packs block info values into a codeword
 * Arguments : Block holding block info
 * Returns : Nothing
 * Notes : Checks that block info values will fit into width
 * Memory allocation : None
 * C.R.Es : C.R.E if value wont fit into codeword
 */
void make_codeword(block_info block);

