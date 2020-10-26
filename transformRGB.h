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
#include "closure_struct.h"

void convert_to_CVS(Pnm_ppm pixmap);

void RGB_to_CVS(int col, int row, A2Methods_UArray2 array2,
                A2Methods_Object *ptr, void *cl_struct);

Pnm_CVS make_CVS(Pnm_rgb RGB, unsigned denominator);