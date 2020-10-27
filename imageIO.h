/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*                           ImageIO.h                                  *
*                                                                      * 
*       Assignment: arith                                              *
*       Authors: Emily Gjertsson (egjert01) & Elise Kaplan (ekapla04)  *  
*       Date: 10/26/2020                                               *
*                                                                      *
*       Summary                                                        *
*           interface of ImageIO.c                                     *
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

#include "transformRGB.h"
#include "compress40.h"

/* Purpose : Read file into ppm and prepare ppm to be compressed by ensuring
 *           usable pixmap dimensions
 * Arguments : File pointer
 * Returns : A Pnm_ppm struct containing the initial image
 * Notes : Assert that the Pnm_ppm and methods is not NULL
 * Memory allocation : Memory allocated for Pnm_ppm, A2Methods_UArray2,
 *                     A2Methods_T, and Pnm_rgb struct
 * C.R.Es : C.R.E if image is not in correct format or Pnm_ppm, methods,  
 *          or input are null
 */
Pnm_ppm read_pixmap(FILE *input);


/* Purpose : Apply function that copies initial pixmap array into a 
 *           new array with an even width and height
 * Arguments : Column, row, original UArray2, Pnm_rgb element, and
 *             destination UArray2
 * Returns : Nothing
 * Notes : Assert that rgb element and destination array are not null
 * Memory allocation : None
 * C.R.Es : C.R.E if rgb elem or destination array are null
 */
void copy_array(int col, int row, A2Methods_UArray2 src_img,
                A2Methods_Object *rgb_elem, void *dest_array);


/* Purpose : Makes new instance of UArray2 with even dimensions
 * Arguments : Pnm_ppm, width, and height
 * Returns : Nothing
 * Notes : None
 * Memory allocation : Old array with odd dimensions freed, new memory
 *                     for even dimension array allocated
 * C.R.Es : None
 */
void trim_image(Pnm_ppm pixmap, int width, int height);

/* Purpose : Determines if width and height need to be trimmed
 * Arguments : Pixmap
 * Returns : Nothing
 * Notes : Assert that width and height are at least 2
 * Memory allocation : None
 * C.R.Es : C.R.E if width and height are less than 2
 */
void valid_dimensions(Pnm_ppm pixmap);
