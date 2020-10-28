/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*                           transformRGB.h                             *
*                                                                      * 
*       Assignment: arith                                              *
*       Authors: Emily Gjertsson (egjert01) & Elise Kaplan (ekapla04)  *  
*       Date: 10/26/2020                                               *
*                                                                      *
*       Summary                                                        *
*           interface of transformRGB.c                                *
*                                                                      *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "assert.h"
#include "pnm.h"
#include "a2methods.h"
#include "a2plain.h"
#include "arith40.h"

#include "compression.h"
#include "compress40.h"

extern const long double BCD_MAX_RANGE, BCD_MIN_RANGE;
extern const int Y_MIN, Y_MAX;
/* 
 * Purpose: Take the given pixmap full of Pnm_rgb structs and call
 *          helper function to create CVS array and populate it with 
 *          Pnm_CVS structs corresponding to the Pnm_rgb struct in the 
 *          pixmap. 
 * Arguments: Pnm_ppm pixmap
 * Returns: None (void)
 * Mem alloc: helper function allocates space for CVS_array, which 
 *            replaces the pixmap->pixels after freeing pixmap->pixels
 * C.R.E.:  raised if pixmap is NULL
 */
void convert_to_CVS(Pnm_ppm pixmap);

