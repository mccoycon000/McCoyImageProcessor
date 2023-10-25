/**
*This is a completed verison of an BMP(24bpp format) image processor. You have to compile and run from command line.
 * First argument must be the file name of image to process
 * Optional arguments are -r,-b,-g, followed by shift values,
 * -o followed by output file name, -s followed by float to scale by
 * -w will gray scale image.
*
* Completion time: 6hrs + 6hrs bug fixes :)
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

    //Checking if file type is correct
    if(strcmp(&(file_input_name[strlen(file_input_name)-4]), ".bmp") != 0){
        printf("Input file must be of type .bmp\n");
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
    int i = 2;
    //Keep track of whether user set a name for output file, which is optional
    int changedName = 0;

    char* file_output_name;

    //Parse through command line arguments//////////////////////////////////////////////////////////////////////////////
    while (argv[i] != NULL){

        //For error checking, endptr stores last value of string parsed by strtol.
        char *endptr;

        //If a valid command is entered valid will be set to 1.
        int valid = 0;

        //Apply color shift/////////////////////////////////////////////////////////////////////////////////////////////
        if (strcmp(argv[i], "-r") == 0){
            valid = 1;
            //If there wasn't a value entered follwing -r -b -g send error message
            if(argv[i+1] == NULL){
                printf("Please enter a valid integer value for color shift following -r\n");
                break;
            }
            //Use strtol to confirm a correct value was entered to color shift by
            int shift = (int)strtol(argv[++i], &endptr, 10);
            //If a non integer was entered send error message.
            // If endptr is '0' that indicates strtol only encountered numbers
            if(*endptr == '\0'){
                image_apply_colorshift(img, shift, 0, 0);
            }else{
                printf("Invalid color shift value, please enter a valid integer following -r \n");
            }
        }

        if (strcmp(argv[i], "-g") == 0){
            valid = 1;
            if(argv[i+1] == NULL){
                printf("Please enter a valid integer value for color shift following -g\n");
                break;
            }
            int shift = (int)strtol(argv[++i], &endptr, 10);
            if(*endptr == '\0'){
                image_apply_colorshift(img, 0, shift, 0);
            }else{
                printf("Invalid color shift value, please enter a valid integer following -g\n");
            }
        }

        if (strcmp(argv[i], "-b") == 0){
            valid = 1;
            if(argv[i+1] == NULL){
                printf("Please enter a valid integer value for color shift following -b\n");
                break;
            }
            int shift = (int)strtol(argv[++i], &endptr, 10);
            if(*endptr == '\0'){
                image_apply_colorshift(img, 0, 0, shift);
            }else{
                printf("Invalid color shift value, please enter a valid integer following -b\n");
            }
        }

        //Apply gray scale//////////////////////////////////////////////////////////////////////////////////////////////
        if (strcmp(argv[i], "-w") == 0){
            valid = 1;
            image_apply_bw(img);
        }

        //Apply resize//////////////////////////////////////////////////////////////////////////////////////////////////
        if (strcmp(argv[i], "-s") == 0){
            valid = 1;
            //Same as for color shift, if there wasn't a value enter after resize send error message
            if(argv[i+1] == NULL){
                printf("Please enter positive float value for resize factor following -s\n");
                break;
            }
            float shift = strtof(argv[++i], &endptr);
            //Make sure that a correct float value was entered
            if(*endptr == '\0' && shift > 0){
                image_apply_resize(img, shift) ;
            }else{
                printf("Invalid value for resize factor, please enter a positive non-zero float value \n");
            }
        }

        //Update file output name///////////////////////////////////////////////////////////////////////////////////////
        if (strcmp(argv[i], "-o") == 0){
            valid = 1;
            file_output_name = argv[++i];
            //Making sure output file type is labeled correctly
            if(strcmp(&(file_output_name[strlen(file_output_name)-4]), ".bmp") != 0){
                printf("Output file must be of type .bmp\n");
                exit(1);
            }
            changedName = 1;
        }

        //If an invalid command in entered send message listing off correct commands to user
        if(valid == 0){
            printf("'%s' Is not a valid command!\nEnter:\n-w, to gray scale image.\n"
                   "-r,-b, or -g followed by integer for color shift.\n"
                   "-s followed by positive float value for resizing.\n"
                   "-o followed by desired file name to set output file name\n", argv[i]);
            break;
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
