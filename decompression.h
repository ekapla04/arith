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


/* 
 * Purpose: Read file into ppm with dimensions from the header in the given 
 *          file. Prepare ppm to be decompressed by ensuring
 *          usable pixmap dimensions
 * Arguments: File pointer
 * Returns: The created pixmap
 * Mem alloc: For a 2d array storing Pnm_rgb structs
 * C.R.E.: if any of the following are NULL - the file pointer 
 *          passed to the function, the pixmap or the 2d array
 */
Pnm_ppm make_pixmap(FILE *input);

/* 
 * Purpose: Read from the file, collecting information byte-by-byte 
 *          and grouping them into 32-bit sequences (words), calls 
 *          unpack_codeword to fill CVS array with the data
 * Arguments: Array holding Pnm_CVS structs, file pointer
 * Returns: None (void)
 * Mem alloc: none
 * C.R.E.:  //TODO
 */
void read_codeword(A2Methods_UArray2 CVS_array, FILE *input); 

/* 
 * Purpose: Take given values, and check if the given value (to_round)
 *          is in the range from the arg low to the arg high. If the value 
 *          is out of bounds, return either the lower or upper bound 
 *          (whichever is closer). Otherwise return the value itself.
 * Returns: None (void)
 * Mem alloc: none
 * C.R.E.: None
 */
long double range_check(long double to_round, long double low, long double high);
