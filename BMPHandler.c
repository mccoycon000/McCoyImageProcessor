

#include <stdio.h>
#include "Image.h"
#include "BMPHandler.h"


/**
* Read BMP header of a BMP file.
*
* @param file: A pointer to the file being read
* @param header: Pointer to the destination BMP header
*/
void readBMPHeader(FILE* file, struct BMP_Header* header){
    fread(&header->signature, sizeof(char)*2, 1, file);
    fread(&header->size, sizeof(int), 1, file);
    fread(&header->reserved1, sizeof(short), 1, file);
    fread(&header->reserved2, sizeof(short), 1, file);
    fread(&header->offset_pixel_array, sizeof(int), 1, file);
}
/**
* Write BMP header of a file. Useful for creating a BMP file.
*
* @param file: A pointer to the file being written
* @param header: The header to write to the file
*/
void writeBMPHeader(FILE* file, struct BMP_Header* header){
    fwrite(&header->signature, sizeof(char)*2, 1, file);
    fwrite(&header->size, sizeof(int), 1, file);
    fwrite(&header->reserved1, sizeof(short), 1, file);
    fwrite(&header->reserved2, sizeof(short), 1, file);
    fwrite(&header->offset_pixel_array, sizeof(int), 1, file);
}
/**
* Read DIB header from a BMP file.
*
* @param file: A pointer to the file being read
* @param header: Pointer to the destination DIB header
*/
void readDIBHeader(FILE* file, struct DIB_Header* header){
    fread(&header->header_size, sizeof(int), 1, file);
    fread(&header->width, sizeof(int), 1, file);
    fread(&header->height, sizeof(int), 1, file);
    fread(&header->planes, sizeof(short), 1, file);
    fread(&header->bit_per_pixel, sizeof(short ), 1, file);
    fread(&header->compression, sizeof(int), 1, file);
    fread(&header->image_size, sizeof(int ), 1, file);
    fread(&header->x_pixels_per_meter, sizeof(int ), 1, file);
    fread(&header->y_pixels_per_meter, sizeof(int ), 1, file);
    fread(&header->colors_in_table, sizeof(int ), 1, file);
    fread(&header->important_color_count, sizeof(int ), 1, file);
}
/**
* Write DIB header of a file. Useful for creating a BMP file.
*
* @param file: A pointer to the file being written
* @param header: The header to write to the file
*/
void writeDIBHeader(FILE* file, struct DIB_Header* header){
    fwrite(&header->header_size, sizeof(int), 1, file);
    fwrite(&header->width, sizeof(int), 1, file);
    fwrite(&header->height, sizeof(int), 1, file);
    fwrite(&header->planes, sizeof(short), 1, file);
    fwrite(&header->bit_per_pixel, sizeof(short ), 1, file);
    fwrite(&header->compression, sizeof(int), 1, file);
    fwrite(&header->image_size, sizeof(int ), 1, file);
    fwrite(&header->x_pixels_per_meter, sizeof(int ), 1, file);
    fwrite(&header->y_pixels_per_meter, sizeof(int ), 1, file);
    fwrite(&header->colors_in_table, sizeof(int ), 1, file);
    fwrite(&header->important_color_count, sizeof(int ), 1, file);
}
/**
* Make BMP header based on width and height. Useful for creating a BMP file.
*
* @param header: Pointer to the destination DIB header
* @param width: Width of the image that this header is for
* @param height: Height of the image that this header is for
*/
void makeBMPHeader(struct BMP_Header* header, int width, int height){
    header->size = (int)((sizeof(struct BMP_Header)) + (sizeof(struct DIB_Header)) + (((3*width)+(4-((3*width)%4)))*height));
}
/**
* Make new DIB header based on width and height.Useful for creating a BMP file.
*
* @param header: Pointer to the destination DIB header
* @param width: Width of the image that this header is for
* @param height: Height of the image that this header is for
*/
void makeDIBHeader(struct DIB_Header* header, int width, int height){
    //Calculate the size of each pixel
    int pixelSize = sizeof(struct Pixel);
    //Find padding based on pixel size and width. Padding will give number to add to each row to make it a multiple of 4
    int padding = (4-((width * pixelSize) % 4))%4;
    header->height = height;
    header->width = width;
    header->image_size = (((pixelSize*width)+padding)*height);

}
/**
* Read Pixels from BMP file based on width and height.
*
* @param file: A pointer to the file being read
* @param pArr: Pixel array to store the pixels being read
* @param width: Width of the pixel array of this image
* @param height: Height of the pixel array of this image
*/
void readPixelsBMP(FILE* file, struct Pixel** pArr, int width, int height){
    int pixelSize = sizeof(struct Pixel);
    //Traverse through the pixel array
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            fread(&pArr[i][j].b, sizeof(unsigned char), 1, file);
            fread(&pArr[i][j].g, sizeof(unsigned char), 1, file);
            fread(&pArr[i][j].r, sizeof(unsigned char), 1, file);
        }
        //Find number to pad each row by
        int padding = (width*pixelSize)%4;
        //Only pad each row if the row isnt already a multiple of 4
        if(padding !=0){
            //Have to subtract padding from 4 because it is added to the row not taken away from it
            for(int x = 0; x < (4-padding); x++) {
                fseek(file, sizeof(unsigned char), SEEK_CUR);
        }
        }
    }
}
/**
* Write Pixels from BMP file based on width and height.
*
* @param file: A pointer to the file being read or written
* @param pArr: Pixel array of the image to write to the file
* @param width: Width of the pixel array of this image
* @param height: Height of the pixel array of this image
*/
void writePixelsBMP(FILE* file, struct Pixel** pArr, int width, int height){
    int pixelSize = sizeof(struct Pixel);
    //Char to use for adding to file for padding
    char* zero = 0;
    //Traverse through pixel array and write it to the file
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            fwrite(&pArr[i][j].b, sizeof(unsigned char), 1, file);
            fwrite(&pArr[i][j].g, sizeof(unsigned char), 1, file);
            fwrite(&pArr[i][j].r, sizeof(unsigned char), 1, file);
        }
        //Padding based on width and pixel size
        int padding = (width*pixelSize)%4;
        //Only pad if its not already a multiple of 4
        if(padding !=0){
            //Subtract padding from 4 so we can add it to the end of the row properly
            for(int x = 0; x < (4-padding); x++) {
                fwrite(&zero, sizeof(unsigned char ), 1, file);
            }
        }

    }
}