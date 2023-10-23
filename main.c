/**
*This is a completed verison of an BMP(24bpp format) image processor. You have to compile and run from command line.
 * First argument must be the file name of image to process
 * Optional arguments are -r,-b,-g, followed by shift values,
 * -o followed by output file name, -s followed by float to scale by
 * -w will gray scale image.
*
* Completion time: 6hrs
*
* @author Connor McCoy, Acuna
* @version 10.22.23
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

    struct BMP_Header BMP;
    struct DIB_Header DIB;

    char* file_input_name = "ttt-1.bmp";
    if(argv[1] != NULL){
        file_input_name = argv[1];
    }
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
    char *file_output_name = strcat(file_input_name, "_copy");

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

}
