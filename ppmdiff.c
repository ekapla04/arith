#include <stdlib.h>
#include <stdio.h>
#include "pnm.h"
#include "assert.h"
#include "a2methods.h"
#include "a2plain.h"
#include "stdbool.h"
#include "string.h"
#include <math.h>

extern Pnm_ppm process_file(FILE *fp);
extern bool compare_images(Pnm_ppm image1, Pnm_ppm image2);
extern float calculate_diff(Pnm_ppm image1, Pnm_ppm image2);

int main(int argc, char *argv[])
{
        Pnm_ppm image1 = NULL;
        Pnm_ppm image2 = NULL;
        if ((argc < 3) || (argc > 3)) {
            printf("incorrect num arguments\n");
            exit(1);
        } else {
            if (strcmp(argv[1], "-") == 0 && strcmp(argv[2], "-") != 0){
                image1 = process_file(stdin);
                image2 = process_file(fopen(argv[2], "r"));
            }
            else if (strcmp(argv[1], "-") != 0 && strcmp(argv[2], "-") == 0){
                image1 = process_file(fopen(argv[1], "r"));
                image2 = process_file(stdin);
            }
            else {
                image1 = process_file(fopen(argv[1], "r"));
                image2 = process_file(fopen(argv[2], "r"));
            }
        }

        bool comparison = compare_images(image1, image2);
        if (comparison) {
            float difference = calculate_diff(image1, image2);
            printf("%0.4f\n", difference);
        }
        return EXIT_SUCCESS;
}

extern Pnm_ppm process_file(FILE *fp){
    if (fp == NULL) {
        fprintf(stderr, "Could not open file for reading \n");
        exit(1);
    }
    A2Methods_T methods = uarray2_methods_plain; 
    assert(methods);
    Pnm_ppm image = Pnm_ppmread(fp, methods);

    fclose(fp);
    return image;
}

extern bool compare_images(Pnm_ppm image1, Pnm_ppm image2){
    if ((image1->width - image2->width) > 1)  {
        fprintf(stderr, "image widths differ by more than 1 \n");
        printf("1.0\n");
        return false;
    }
    else if ((image1->height - image2->height) > 1) {
        fprintf(stderr, "image height differ by more than 1 \n");
        printf("1.0\n");
        return false;
    }
    return true;
}

extern float calculate_diff(Pnm_ppm image1, Pnm_ppm image2){
    float red, green, blue, color_sum = 0;
    for (unsigned int i = 0; i < image1->width; i++)
    {
        for (unsigned int j = 0; j < image1->height; j++)
        {
            Pnm_rgb colors1 = image1->methods->at(image1->pixels, i, j);
            Pnm_rgb colors2 = image2->methods->at(image2->pixels, i, j);
            red = ((float)colors1->red - (float)colors2->red);
            green = ((float)colors1->green - (float)colors2->green);
            blue = ((float)colors1->blue - (float)colors2->blue);
            // printf("color1: % \ncolor2: %f\n", colors1->blue, colors2->blue);
            // printf("red: %0.4f\n", red);
            // printf("green: %0.4f\n", green);
            printf("blue: %0.4f\n", blue);
            color_sum += (red * red) + (green * green) + (blue * blue);
            printf("color sum: %0.4f\n", color_sum); 
        }  
    }
    
    color_sum = (color_sum / (3 * image1->width * image1->height));
    color_sum = sqrt(color_sum);
    color_sum = color_sum/100;


    return color_sum;
}

