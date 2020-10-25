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

    if (Bitpack_fitss(-12, 5)){
        printf("signed true\n");
    } else {
        printf("signed false\n");
    }
    

    return 0;
}

