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

extern const long double SCALE_FACTOR, A_FACTOR;

/* 
 * Purpose: handle all compression that happens block-by-block. Prints 
 *          compressed data to standard output.
 * Ouput Notes: Series of bitpacked uints, printed a byte at a time
 * Arguments: Array storing Pnm_CVS struct to be compressed
 * Returns: None(void)
 * Mem alloc: None
 * C.R.E.: if the given array is NULL
 */
void compress_by_block(A2Methods_UArray2 CVS_array);

