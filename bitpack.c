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

Except_T Bitpack_Overflow = { "Overflow packing bits" };

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
    Bitpack.c deals with making masks and shifting values to
    effectively pack information into 32 bits
    C.R.Es : C.R.E if bitpack overflow conditions met and 
             value doesn't fit in width
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

uint64_t make_mask(unsigned width, unsigned lsb) 
{
    uint64_t mask = ~0;
    mask = mask >> (64 - width);
    mask = mask << lsb;
    return ~mask;
}

/*
2^num bits(width) = num ints that can be represented (starting at 0). 
If n exists in this range, should return true

Your interface will test to see if an integer can be represented in k bits. 
The answer will depend on whether the k bits
are interpreted as unsigned integers or as signed integers in the two’s-complement 
representation. We will refer to
these representations using the shorthand “k unsigned bits” and “k signed bits.” 

The functions tell whether the argument n can be represented in width bits. For example, 3 bits can represent unsigned
integers in the range 0 to 7, so Bitpack fitsu(5, 3) == true. But 3 bits can represent signed integers only in the
range −4 to 3, so Bitpack fitss(5, 3) == false
*/
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

/* 2 to the power of width represenths the number of integers that can
   be represented. To get the starting point, divide by 2 and multiply by 
   negative 1. */ 
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

//move left how many bits are left after removing everythings that's on the right
//of the word, and the width of the word
    word = word << (64 - (width + lsb)); 
//move right until just the word is left
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

    //make a mask to clear out existing elements in slot
    //take our int and shift it to the correct spot
    //add our int to the slot

    uint64_t mask = make_mask(width, lsb);

    word = word & mask; //clear the spot to add info into
    value = value << lsb;

    return (word | value);
}

uint64_t Bitpack_news(uint64_t word, unsigned width, 
                      unsigned lsb,  int64_t value)
{
    assert (width > 0 && width + lsb <= 64); //TODO: add a check, ensure width > 0

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