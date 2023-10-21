

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
    header->size = (sizeof(struct BMP_Header)) + (sizeof(struct DIB_Header)) + (((3*width)+((3*width)%4))*height);
}
/**
* Make new DIB header based on width and height.Useful for creating a BMP file.
*
* @param header: Pointer to the destination DIB header
* @param width: Width of the image that this header is for
* @param height: Height of the image that this header is for
*/
void makeDIBHeader(struct DIB_Header* header, int width, int height){
    header->height = height;
    header->width = width;
    header->image_size = (((3*width)+((3*width)%4))*height);

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
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            fread(&pArr[i][j].b, sizeof(unsigned char), 1, file);
            fread(&pArr[i][j].g, sizeof(unsigned char), 1, file);
            fread(&pArr[i][j].r, sizeof(unsigned char), 1, file);
        }
        for(int x = 0; x < ((width*3)%4); x++) {//This is for padding based on # of pixels in row
            fseek(file, sizeof(unsigned char), SEEK_CUR);
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
    char* zero = 0;
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            fwrite(&pArr[i][j].b, sizeof(unsigned char), 1, file);
            fwrite(&pArr[i][j].g, sizeof(unsigned char), 1, file);
            fwrite(&pArr[i][j].r, sizeof(unsigned char), 1, file);
        }
        for(int x = 0; x < ((width*3)%4); x++) {//This is for padding based on # of pixels in row
            fwrite(zero, sizeof(char ), 1, file);
        }
    }
}