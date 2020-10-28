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

Except_T Bitpack_Overflow = { "Overflow packing bits" }; //this is in the .h do I need to include it?
/* 
 * Purpose: create an unsigned integer whose bits are filled with 1's apart
 *          from the bits starting from lsb and spanning width
 * Arguments: width (how many bits will be 0) and lsb (starting point)
 * Returns: unsigned integer holding the mask
 * C.R.E.:  none
 */
uint64_t make_mask(unsigned width, unsigned lsb) 
{
    uint64_t mask = ~0;
    mask = mask >> (64 - width);
    mask = mask << lsb;
    return ~mask;
}


/* 
 * Purpose: Test to see if an integer can be represented in an unsigned
 *          integer in width bits
 * Arguments: the value to store, and the number of bits available for storing
 * Returns: bool, true if the value fits in the width, false if not
 * C.R.E.:  if width is 0 or if width is greater than 64 (the available space)
 */
bool Bitpack_fitsu(uint64_t value, unsigned width)
{
    assert (width != 0 && width <= 64);
    int int_range = pow(2, width);

    if (value < (uint64_t)int_range) {
        return true;
    } else {
        return false;
    }
}

//2^num bits(width) = num ints that can be represented. Divide by 2 and * -1, 
//get the starting point. If n exists in this range, return true, otherwise return false
/* 
 * Purpose: Test to see if an integer can be represented in a signed
 *          integer in width bits
 * Arguments: the value to store, and the number of bits available for storing
 * Returns: bool, true if the value fits in the width, false if not
 * C.R.E.:  if width is 0 or if width is greater than 64 (the available space)
 */
bool Bitpack_fitss(int64_t value, unsigned width)
{
    assert (width != 0 && width <= 64);
    int int_range = pow(2, width);
    int start_pt = int_range / 2 * (-1);

    if (value >= start_pt && value < start_pt + int_range) {
        return true;
    } else {
        return false;
    }
}

/* 
 * Purpose: retrieve a field of bits from an unsigned integer (word) 
 * Arguments: word (the unsigned to extract values from), width (number of 
 *            bits to be extracted), and lsb (starting point in the word)
 * Returns: unsigned integer holding just the retrieved value
 * C.R.E.:  if width is 0 or less, or if width + lsb is greater than 64 
 *          (the available space)
 */
uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb)
{
    assert (width > 0 && width + lsb <= 64); 

    word = word << (64 - (width + lsb)); 
    word = word >> (64 - width);

    return (word);

    return 0;
}

/* 
 * Purpose: retrieve a field of bits from a signed integer (word) 
 * Arguments: word (the unsigned to extract values from), width (number of 
 *            bits to be extracted), and lsb (starting point in the word)
 * Returns: signed integer holding just the retrieved value
 * C.R.E.:  if width is 0 or less, or if width + lsb is greater than 64 
 *          (the available space)
 */
int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb)
{
    assert (width > 0 && width + lsb <= 64); 

    word = (int64_t)word << (64 - (width + lsb));
    word = (int64_t)word >> (64 - width);
    
    return word;
}

/* 
 * Purpose: function for creating a new bitpacked unsigned integer or for 
 *          updating the field of one that already exists
 * Arguments: word (the unsigned to put values into), width (length of the 
 *            field), lsb (starting point in the word), and value (what is 
 *            being put in the field -- can only be positive to fit)
 * Returns: the created or updated unsigned integer
 * C.R.E.:  if width is 0 or less, or if width + lsb is greater than 64 
 *          (the available space)
 */
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

/* 
 * Purpose: function for creating a new bitpacked signed integer or for 
 *          updating the field of one that already exists
 * Arguments: word (the unsigned int to put values into), width (length of the 
 *            field), lsb (starting point in the word), and value (what is 
 *            being put in the field -- can be positive or negative)
 * Returns: the created or updated unsigned integer
 * C.R.E.:  if width is 0 or less, or if width + lsb is greater than 64 
 *          (the available space)
 */
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