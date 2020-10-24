#include "compress40.h"
#include <stdlib.h>
#include <stdio.h>
#include "pnm.h"
#include "assert.h"
#include "a2methods.h"
#include "a2plain.h"
#include "stdbool.h"
#include "string.h"
#include "arith40.h"

#include "decompression.h"

void compress_by_block(A2Methods_UArray2 VCS_array);

block_info quantize_chroma(Pnm_VCS pixel1, Pnm_VCS pixel2, 
                           Pnm_VCS pixel3, Pnm_VCS pixel4);

void perform_DCT(Pnm_VCS pixel1, Pnm_VCS pixel2, 
                       Pnm_VCS pixel3, Pnm_VCS pixel4, block_info block);


