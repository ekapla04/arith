#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "assert.h"
#include "except.h"
#include "bitpack.h"

Except_T Bitpack_Overflow = { "Overflow packing bits" }; //this is in the .h do I need to include it?

uint64_t make_mask(unsigned width, unsigned lsb){
    uint64_t mask_h = ~(0), mask_l = ~(0); //64 bits just 1s
    mask_h = mask_h << (64 - (width + lsb));
    mask_l = mask_l >> lsb;
    return mask_l & mask_h;
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


    printf("n: %ld\n", n);
    printf("possible range: ");
    for (int i = 0; i < int_range; i++){
        printf("%d, ", i);
    } 
    printf("\n");


    if (n < int_range){
        return true;
    } else {
        return false;
    }
}

//2^num bits(width) = num ints that can be represented. Divide by 2 and * -1, 
//get the starting point. If n exists in this range, return true, otherwise return false
bool Bitpack_fitss(int64_t n, unsigned width)
{
    assert (width != 0 && width <= 64);
    int int_range = pow(2, width);
    int start_pt = int_range / 2 * (-1);


    printf("n: %ld\n", n);
    printf("possible range: ");
    for (int i = start_pt; i < start_pt + int_range; i++){
        printf("%d, ", i);
    } 
    printf("\n");


    if (n >= start_pt && n < start_pt + int_range){
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
    
    printf("word: %ld\n", word);

    return word;
}

uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb, 
                                                    uint64_t value)
{
    assert (width <= 64 && width + lsb <= 64); 

    if (!Bitpack_fitsu(value, width)) {
        printf("NO! (unsigned)\n");
        RAISE(Bitpack_Overflow);  
    }

    printf("fits\n");

    //make a mask to clear out existing elements in slot
    //take our int and shift it to the correct spot
    //add our int to the slot

    uint64_t mask = make_mask(width, lsb);

    word = word & mask; //clear the spot to add info into
    value = value << lsb;

    return (word | value);
}

uint64_t Bitpack_news(uint64_t word, unsigned width, unsigned lsb,  int64_t value)
{
    assert (width <= 64 && width + lsb <= 64); 

    if (!Bitpack_fitss(value, width)) {
        printf("NO! (signed)\n");
        RAISE(Bitpack_Overflow);  
    }
    printf("fits\n");

    if(value >= 0){
        printf("POSITIVE NUMBER: %ld\n", value);
        return Bitpack_newu(word, width, lsb, value);
    }

    uint64_t mask = make_mask(width, lsb);
    word = word & mask;

    uint64_t neg_bit_mask = make_mask(1, lsb);
    neg_bit_mask = neg_bit_mask << (width - 1);

    word = word & neg_bit_mask;

    value = (uint64_t)value;
    value = value << lsb;

    return (word | value);

}