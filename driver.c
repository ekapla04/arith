#include "compress40.h"
// #include "compression.h"
// #include "decompression.h"
#include <stdlib.h>
#include <stdio.h>
#include "pnm.h"
#include "assert.h"
#include "a2methods.h"
#include "a2plain.h"
#include "stdbool.h"
#include "string.h"

static void usage(const char *progname)
{
    fprintf(stderr, "Usage: 40image -d %s\n       40image -c %s\n",
                    progname, progname);
    exit(1);
}

int main(int argc, char *argv[])
{
    FILE *fp;
    if (argc > 3) {
        usage(argv[2]);
    } 
    else if (argc == 3) {
        fp = fopen(argv[2], "r");
        if (fp == NULL) {
            fprintf(stderr, 
                "ERROR: The file could not be opened for reading\n");
            exit(EXIT_FAILURE);
        }
    }
    else {
        fp = stdin;
    } 
    if (strcmp(argv[1], "-c") == 0){
        // call compression
        compress40(fp);
    }
    else if (strcmp(argv[1], "-d") == 0) {
        decompress40(fp);
    } else {
        fclose(fp);
        usage(argv[2]);
    }
    fclose(fp);
    /* initialize pixmap */
    // A2Methods_T methods = uarray2_methods_plain; 
    // assert(methods);
    // Pnm_ppm image = Pnm_ppmread(fp, methods);
    return 0;
}

