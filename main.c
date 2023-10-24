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

    //Default file name so program compiles from IDE
    char* file_input_name = "ttt-1.bmp";
    //Parse file name given through command line

    file_input_name = argv[1];
    //Checking if file entered exists
    if (access(file_input_name, F_OK) != 0) {
        printf("That file doesn't exist, please enter correct file name as first argument\n");
        exit(1);
    }
    //Open file given from command line
    FILE* file_input = fopen(file_input_name, "rb");

    //Populate header structs
    readBMPHeader(file_input, &BMP);
    readDIBHeader(file_input, &DIB);


    //Allocate memory for pixel array
    struct Pixel** pixels = (struct Pixel**)malloc(sizeof(struct Pixel*) * DIB.height);
    for (int p = 0; p < DIB.width; p++) {
        pixels[p] = (struct Pixel*)malloc(sizeof(struct Pixel) * DIB.width);
    }

    //Read pixel data from bmp image file
    readPixelsBMP(file_input, pixels, DIB.width, DIB.width);
    //Close file, done reading
    fclose(file_input);
    //Create image based on info parsed from file
    Image* img = image_create(pixels, DIB.width, DIB.height);

    //Value for indexing arg array
    int i = 0;
    //Keep track of whether user set a name for output file, which is optional
    int changedName = 0;

    char* file_output_name;
    //Parse through command line arguments
    while (argv[i] != NULL){
        char *endptr;
        //Apply color shift
        if (strcmp(argv[i], "-r") == 0){
            if(argv[i+1] == NULL){
                printf("Please enter integer value for color shift following -r\n");
                break;
            }
            int shift = (int)strtol(argv[i+1], &endptr, 10);
            if(*endptr == '\0'){
                image_apply_colorshift(img, shift, 0, 0);
            }else{
                printf("Invalid color shift value, please enter an integer \n");
            }
        }
        if (strcmp(argv[i], "-g") == 0){
            if(argv[i+1] == NULL){
                printf("Please enter integer value for color shift following -g\n");
                break;
            }
            int shift = (int)strtol(argv[i+1], &endptr, 10);
            if(*endptr == '\0'){
                image_apply_colorshift(img, 0, shift, 0);
            }else{
                printf("Invalid color shift value, please enter an integer \n");
            }
        }
        if (strcmp(argv[i], "-b") == 0){
            if(argv[i+1] == NULL){
                printf("Please enter integer value for color shift following -b\n");
                break;
            }
            int shift = (int)strtol(argv[i+1], &endptr, 10);
            if(*endptr == '\0'){
                image_apply_colorshift(img, 0, 0, shift);
            }else{
                printf("Invalid color shift value, please enter an integer \n");
            }
        }
        //Apply gray scale
        if (strcmp(argv[i], "-w") == 0){
            image_apply_bw(img);
        }
        //Apply resize
        if (strcmp(argv[i], "-s") == 0){
            if(argv[i+1] == NULL){
                printf("Please enter float value for resize factor following -s\n");
                break;
            }
            float shift = strtof(argv[i+1], &endptr);
            if(*endptr == '\0'){
                image_apply_resize(img, shift) ;
            }else{
                printf("Invalid value for factor, please enter a valid float value \n");
            }
        }
        //Update file output name based on name given through command line
        if (strcmp(argv[i], "-o") == 0){
            file_output_name = argv[i+1];
            changedName = 1;
        }
        i++;
    }
    //If output name wasn't given through command line make it default to original name + _copy.bmp
    if(changedName ==0){
        file_input_name[strlen(file_input_name)-4] = '\0';
        file_output_name = strcat(file_input_name, "_copy.bmp");
    }
    //Update the headers
    makeBMPHeader(&BMP, image_get_width(img), image_get_height(img));
    makeDIBHeader(&DIB, image_get_width(img), image_get_height(img));
    //Open output file for writing
    FILE* file_output = fopen(file_output_name, "wb");
    //Write headers to file
    writeBMPHeader(file_output, &BMP);
    writeDIBHeader(file_output, &DIB);
    //Right pixel array to file
    writePixelsBMP(file_output, image_get_pixels(img), image_get_width(img),
                   image_get_width(img));
    //Free image memory
    image_destroy(&img);
    fclose(file_output);

}
