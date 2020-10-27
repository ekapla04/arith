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


void CVS_to_RGB(int col, int row, A2Methods_UArray2 src_img,
                A2Methods_Object *cvs_elem, void *cl);

struct Pnm_rgb make_RGB(Pnm_CVS CVS);

long double round2(long double to_round, long double low, long double high);
