/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
*                           compression.h
*                               
*       Assignment: arith
*       Authors: Emily Gjertsson (egjert01) & Elise Kaplan (ekapla04)    
*       Date: 10/26/2020
*
*       Summary
*           interface of compression.c
*           Handles compression block-by-block
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
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
// #include "Pnm_CVS.h"
// #include "block_info.h"

extern const float SCALE_FACTOR;

void compress_by_block(A2Methods_UArray2 CVS_array);

block_info quantize_chroma(Pnm_CVS pixel1, Pnm_CVS pixel2, 
                           Pnm_CVS pixel3, Pnm_CVS pixel4);

void perform_DCT(Pnm_CVS pixel1, Pnm_CVS pixel2, 
                       Pnm_CVS pixel3, Pnm_CVS pixel4, block_info *block);

int64_t quantize_degree_brightness(float degree);

void make_codeword(block_info block);

