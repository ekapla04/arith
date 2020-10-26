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

Pnm_ppm read_pixmap(FILE *input);

void copy_array(int col, int row, A2Methods_UArray2 src_img,
                A2Methods_Object *rgb_elem, void *dest_array);

void trim_image(Pnm_ppm pixmap, int width, int height);

void valid_dimensions(Pnm_ppm pixmap);
