/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*                           bitpack.c                                  *
*                                                                      * 
*       Assignment: arith                                              *
*       Authors: Emily Gjertsson (egjert01) & Elise Kaplan (ekapla04)  *  
*       Date: 10/26/2020                                               *
*                                                                      *
*       Summary                                                        *
*           implementation of bitpack.h                                *
*           All functions one might need to bitpack -- i.e. encode     *
*           values into a signed or unsigned integer                   *
*                                                                      *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "assert.h"
#include "except.h"
#include "bitpack.h"

/* Functions in this file create masks to pack various fields into a 
   64 bit signed or unsigned integer */

Except_T Bitpack_Overflow = { "Overflow packing bits" }; 

uint64_t make_mask(unsigned width, unsigned lsb) 
{
    uint64_t mask = ~0;
    mask = mask >> (64 - width);
    mask = mask << lsb;
    return ~mask;
}

bool Bitpack_fitsu(uint64_t n, unsigned width)
{
    assert (width != 0 && width <= 64);
    int int_range = pow(2, width);

    if (n < (uint64_t)int_range) {
        return true;
    } else {
        return false;
    }
}

bool Bitpack_fitss(int64_t n, unsigned width)
{
    assert (width != 0 && width <= 64);
    int int_range = pow(2, width);
    int start_pt = int_range / 2 * (-1);

    if (n >= start_pt && n < start_pt + int_range) {
        return true;
    } else {
        return false;
    }
}

uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb)
{
    assert (width > 0 && width <= 64 && width + lsb <= 64); 

    word = word << (64 - (width + lsb)); 
    word = word >> (64 - width);

    return (word);

    return 0;
}

int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb)
{
    assert (width > 0 && width <= 64 && width + lsb <= 64); 

    word = (int64_t)word << (64 - (width + lsb));
    word = (int64_t)word >> (64 - width);
    
    return word;
}

uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb, 
                                                    uint64_t value)
{
    assert (width + lsb <= 64 && width > 0); 

    if (!Bitpack_fitsu(value, width)) {
        RAISE(Bitpack_Overflow);  
    }

    uint64_t mask = make_mask(width, lsb);

    word = word & mask;
    value = value << lsb;

    return (word | value);
}

uint64_t Bitpack_news(uint64_t word, unsigned width, 
                      unsigned lsb,  int64_t value)
{
    assert (width > 0 && width + lsb <= 64);

    if (!Bitpack_fitss(value, width)) {
        RAISE(Bitpack_Overflow);  
    }

    if (value >= 0) {
        return Bitpack_newu(word, width, lsb, value);
    }
    uint64_t mask = make_mask(width, lsb);
    word = word & mask;

    uint64_t leading_1s = ~0;
    leading_1s = leading_1s >> (64 - (width));

    value = value & leading_1s;
    value = value << lsb;

    return (word | value);

}