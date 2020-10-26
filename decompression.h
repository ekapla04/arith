#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "assert.h"
#include "pnm.h"
#include "arith40.h"
#include "a2methods.h"
#include "a2plain.h"

#include "compress40.h"

#include "Pnm_CVS.h"
#include "block_info.h"
#include "bitpack.h"


void unquantize_chroma(block_info block, Pnm_CVS);
// give it quantized values
// get unquantizes values
// return average values --> save them for 2x2 block

void inverse_DCT(Pnm_CVS pixel1, Pnm_CVS pixel2, 
                 Pnm_CVS pixel3, Pnm_CVS pixel4, block_info block);

float unquantize_degree_brightness(int64_t degree);