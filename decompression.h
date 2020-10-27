/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*                           decompression.h                            *
*                                                                      * 
*       Assignment: arith                                              *
*       Authors: Emily Gjertsson (egjert01) & Elise Kaplan (ekapla04)  *  
*       Date: 10/26/2020                                               *
*                                                                      *
*       Summary                                                        *
*           interface of decompression.c                               *
*                                                                      *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "assert.h"
#include "pnm.h"
#include "arith40.h"
#include "a2methods.h"
#include "a2plain.h"

#include "compress40.h"
#include "bitpack.h"

#include "Pnm_CVS.h"
#include "block_info.h"

/* Purpose : 
 * Arguments : A Pnm_ppm struct of the original image, a mapping function and 
 *             the degrees of rotation requested by the client.
 * Returns : A Pnm_ppm struct containing the rotated image.
 * Notes : Assert that the Pnm_ppm and A2Methods_mapfun is not NULL
 * Memory allocation : 
 * C.R.Es : 
 */

long double rounder(long double to_round, long double low, long double high);

Pnm_ppm make_pixmap(FILE *input);

void read_codeword(A2Methods_UArray2 CVS_array, FILE *input); 

Pnm_CVS unquantize_chroma(block_info *block);

void inverse_DCT(Pnm_CVS *pixel1, Pnm_CVS *pixel2, 
                 Pnm_CVS *pixel3, Pnm_CVS *pixel4, block_info *block);

long double unquantize_degree_brightness(int64_t degree);

void unpack_codeword(uint64_t word, int row, int col, 
                    A2Methods_UArray2 CVS_array);

void unquantize_values(Pnm_CVS *pixel1, Pnm_CVS *pixel2, 
                 Pnm_CVS *pixel3, Pnm_CVS *pixel4, block_info *block);

void place_pixels(Pnm_CVS pixel1, Pnm_CVS pixel2, Pnm_CVS pixel3, 
                Pnm_CVS pixel4, A2Methods_UArray2 CVS_array, int col, int row);
