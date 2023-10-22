/**
*This is a completed verison of an BMP image processor. You have to compile and run from comm
*
* Completion time: 334 minutes
*
* @author Vatricia Edgar, Ruben Acuna
* @version 1.0
*/
////////////////////////////////////////////////////////////////////////////////
//INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "BMPHandler.h"
#include "Image.h"

int main(int argc, char** argv) {
    //START - HEADER FUNCTIONALITY MINIMAL EXAMPLE (DO NOT SUBMIT)
    struct BMP_Header BMP;
    struct DIB_Header DIB;

    char* file_input_name = "ttt-1.bmp";
    if(argv[1] != NULL){
        file_input_name = argv[1];
    }
    printf("File name: %s\n", file_input_name);
    FILE* file_input = fopen(file_input_name, "rb");


    readBMPHeader(file_input, &BMP);
    readDIBHeader(file_input, &DIB);


    struct Pixel** pixels = (struct Pixel**)malloc(sizeof(struct Pixel*) * DIB.height);
    for (int p = 0; p < DIB.width; p++) {
        pixels[p] = (struct Pixel*)malloc(sizeof(struct Pixel) * 152);
    }


    readPixelsBMP(file_input, pixels, DIB.width, DIB.width);
    fclose(file_input);
    Image* img = image_create(pixels, DIB.width, DIB.height);

    int i = 2;
    char *file_output_name = "test_output";

    while (argv[i] != NULL){

        if (strcmp(argv[i], "-r") == 0){
            int shift = (int)atof(argv[i+1]);
            image_apply_colorshift(img, shift, 0, 0);
        }
        if (strcmp(argv[i], "-g") == 0){
            int shift = (int)atof(argv[i+1]);
            image_apply_colorshift(img, 0, shift, 0);
        }
        if (strcmp(argv[i], "-b") == 0){
            int shift = (int)atof(argv[i+1]);
            image_apply_colorshift(img, 0, 0, shift);
        }
        if (strcmp(argv[i], "-w") == 0){
            image_apply_bw(img);
        }
        if (strcmp(argv[i], "-s") == 0){
            float shift = atof(argv[i+1]);
            image_apply_resize(img, shift) ;
        }
        if (strcmp(argv[i], "-o") == 0){
            file_output_name = argv[i+1];
        }


        i++;
    }

    int c;

    makeBMPHeader(&BMP, image_get_width(img), image_get_height(img));
    makeDIBHeader(&DIB, image_get_width(img), image_get_height(img));

    FILE* file_output = fopen(file_output_name, "wb");

    writeBMPHeader(file_output, &BMP);
    writeDIBHeader(file_output, &DIB);
    writePixelsBMP(file_output, image_get_pixels(img), image_get_width(img),
                   image_get_width(img));

    image_destroy(&img);
    fclose(file_output);
//END - HEADER FUNCTIONALITY MINIMAL EXAMPLE (DO NOT SUBMIT)
}
