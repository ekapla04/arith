/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*                           transformCVS.h                             *
*                                                                      * 
*       Assignment: arith                                              *
*       Authors: Emily Gjertsson (egjert01) & Elise Kaplan (ekapla04)  *  
*       Date: 10/26/2020                                               *
*                                                                      *
*       Summary                                                        *
*           interface of transformCVS.c                                *
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

#include "imageIO.h"
#include "compress40.h"


/* 
 * Purpose: Apply function to convert component video color 
 *          space values to RGB values. Takes each element holding CVS values,
 *          converts them to RGB values, and puts them into a new array to
 *          hold RGB values
 * Arguments: Column, row, array holding CVS structs, individual CVS element,
 *            closure holding RGB array
 * Returns: None (void)
 * Mem alloc: None
 * C.R.E.: raised if CVS element or closure are null
 */
void CVS_to_RGB(int col, int row, A2Methods_UArray2 src_img,
                A2Methods_Object *cvs_elem, void *cl);