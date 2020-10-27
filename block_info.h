/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*                             block_info.h                             *
*                                                                      * 
*       Assignment: arith                                              *
*       Authors: Emily Gjertsson (egjert01) & Elise Kaplan (ekapla04)  *  
*       Date: 10/26/2020                                               *
*                                                                      *
*       Summary                                                        *
*           struct definition of block_info, holds quantized values    *
*           transformed from component video values                    *
*               PR_4bit & PB_4bit: brightness (between 0 and 1)                        *
*               PB & PR: color-difference signals                      *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include <stdint.h>

typedef struct block_info {
    uint64_t PR_4bit;
    uint64_t PB_4bit;
    uint64_t a;
    int64_t b;
    int64_t c;
    int64_t d;
} block_info;