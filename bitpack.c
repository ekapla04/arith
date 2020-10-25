#include <stdio.h>
#include <math.h>
#include "assert.h"
#include "bitpack.h"


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
uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb){
    (void)word;
    (void)width;
    (void)lsb;
}
// int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb);
uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb, 
                                                    uint64_t value)
{
    assert (width <= 64 && width + lsb <= 64); 

    if (!Bitpack_fitsu(value, width)) {
            RAISE (Bitpack_Overflow);  
            exit (EXIT_FAILURE);
    }

    
}
// uint64_t Bitpack_news(uint64_t word, unsigned width, unsigned lsb,  int64_t value);