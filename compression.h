#include "compress40.h"
#include <stdlib.h>
#include <stdio.h>
#include "pnm.h"
#include "assert.h"
#include "a2methods.h"
#include "a2plain.h"
#include "stdbool.h"
#include "string.h"


void compress_by_block(A2Methods_UArray2 VCS_array);

uint64_t quantize_chroma(A2Methods_UArray2 VCS_array);

