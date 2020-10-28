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
*               PR_4bit & PB_4bit: brightness (between 0 and 1)        *
*               a: the average brightness of the image                 *
*               b: degree to which the image gets brighter as we       *
*                  move from top to bottom                             *
*               c: degree to which the image gets brighter as we       *
*                  move from left to right
*               c: degree to which the pixels on one diagonal are      *
*                  brighter than the pixels on the other diagonal      *
*                                                                      *
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