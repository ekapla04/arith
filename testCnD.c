#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

#include "assert.h"

#include "a2methods.h"
#include "a2plain.h"
#include "pnm.h"

// #include "compress40.h"
#include "compression.h"

void check_bounds(Pnm_rgb RGB){

    if(RGB->red < 0){
        printf("1!!!!\n");
        RGB->red = 0;
    } else if (RGB->green < 0){
        printf("2!!!!\n");
        RGB->green = 0;
    } else if (RGB->blue < 0){
        printf("3!!!!\n");
        RGB->blue = 0;
    }

    //greater than the denominator
    if(RGB->red > 255){
        printf("4!!!!\n");
        RGB->red = 255;
    } else if (RGB->green > 255){
        printf("5!!!!\n");        
        RGB->green = 255;
    } else if (RGB->blue > 255){
        RGB->blue = 255;
        printf("6!!!!\n");
    }
}

float round_f(float to_round, float low, float high){
    if (to_round < low){
        to_round = low;
    } else if (to_round > high){
        to_round = high;
    }

    return to_round;
}

Pnm_CVS make_CVS2(Pnm_rgb RGB, unsigned denominator)
{
    Pnm_CVS CVS_values; 

    /* ensure values lie between 0 and 1*/
    float r = (float)RGB->red;
    float b = (float)RGB->blue;
    float g = (float)RGB->green;

    CVS_values.Y =  ((0.299 * r) + (0.587 * g) + (0.114 * b))/denominator;

    CVS_values.PB = ((-0.168736 * r) - (0.331264 * g) +  (0.5 * b))/denominator;

    CVS_values.PR = ((0.5 * r) - (0.418688 * g) - (0.081312 * b))/denominator;

    // printf("R: %f, G: %f, B: %f ---> ", r, g, b);
    // printf("Y: %f, PB: %f, PR: %f\n", CVS_values.Y, CVS_values.PB, CVS_values.PR);
    CVS_values.Y = round_f(CVS_values.Y, 0, 1);
    CVS_values.PB = round_f(CVS_values.PB, -0.5, 0.5);
    CVS_values.PR = round_f(CVS_values.PR, -0.5, 0.5);
    return CVS_values;
}

Pnm_rgb make_RGB2(Pnm_CVS CVS)
{
    Pnm_rgb RGB_values = malloc(sizeof(struct Pnm_rgb));

    /* WE CHOOSE DENOMINATOR TO BE 255 */
    float y = (float)CVS.Y;
    float pb = (float)CVS.PB;
    float pr = (float)CVS.PR;
    // printf("CVS values - Y: %f, PB: %f, PR: %f \n", y, pb, pr);
    RGB_values->red = (1.0 * y) + (0.0 * pb) + (1.402 * pr);
    RGB_values->green = (1.0 * y) - (0.344136 * pb) - (0.714136 * pr);
    RGB_values->blue = (1.0 * y) + (1.772 * pb) + (0.0 * pr);

    RGB_values->red = round_f(RGB_values->red, 0, 1) * 255;
    RGB_values->green = round_f(RGB_values->green, 0, 1) * 255;
    RGB_values->blue = round_f(RGB_values->blue, 0, 1) * 255;

    
    //check_bounds(RGB_values);

    printf("Y: %f, PB: %f, PR: %f --->", y, pb, pr);
    //printf("original R: %u, G: %u, B: %u\n", RGB_values->red, RGB_values->green, RGB_values->blue);

    //printf("Y: %f, PB: %f, PR: %f\n",  CVS_values->Y,  CVS_values->PB,  CVS_values->PR);
    return RGB_values;
}


int main(){
    printf("howdy pardner\n");

    Pnm_rgb RGB = malloc(sizeof(struct Pnm_rgb));
    RGB->red = 115;
    RGB->green = 0;
    RGB->blue = 50;
    printf("ORIGINAL RGB:        r: %u,        g: %u,        b: %u\n", RGB->red, RGB->green, RGB->blue);
    
    Pnm_CVS values = make_CVS2(RGB, 15);
    printf("...converted to CVS: y: %f, Pb: %f, Pr: %f\n", values.Y, values.PB, values.PR);
    
    RGB = make_RGB2(values);
    printf("REVERTED RGB:        r: %u,        g: %u,        b: %u\n", RGB->red, RGB->green, RGB->blue);

    printf("\n");
    return 0;
}




