#include "compress40.h"
#include <stdlib.h>
#include <stdio.h>
#include "pnm.h"
#include "assert.h"
#include "a2methods.h"
#include "a2plain.h"
#include "stdbool.h"
#include "string.h"
#include "Pnm_VCS.h"
#include "block_info.h"
#include "arith40.h"

void unquantize_chroma(block_info block, Pnm_VCS);
// give it quantized values
// get unquantizes values
// return average values --> save them for 2x2 block

void inverse_DCT(Pnm_VCS pixel1, Pnm_VCS pixel2, 
                 Pnm_VCS pixel3, Pnm_VCS pixel4, block_info block);