#include <stdio.h>
#include "assert.h"
#include "bitpack.h"


int main () {
    printf("testing bitpack\n");

    if (Bitpack_fitsu(-12, 5)){
        printf("unsigned true\n");
    } else {
        printf("unsigned false\n");
    }

    if (Bitpack_fitss(5, 3)){
        printf("unsigned true\n");
    } else {
        printf("unsigned false\n");
    }

    if (Bitpack_fitss(-12, 5)){
        printf("signed true\n");
    } else {
        printf("signed false\n");
    }
    
    printf("\n\n\n");

    //args: uint64_t word, unsigned width, unsigned lsb, uint64_t value
    Bitpack_newu(12, 3, 4, 3);

    printf("val (unsigned): %ld\n", Bitpack_getu(Bitpack_newu(12, 6, 4, 5), 6, 4));
    
    printf("val (signed): %ld\n", Bitpack_gets(Bitpack_news(12, 6, 4, -12), 6, 4));



    return 0;
}

