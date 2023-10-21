//
// Created by connor on 10/20/23.
//

#ifndef MCCOYIMAGEPROCESSOR_BMPHANDLER_H
#define MCCOYIMAGEPROCESSOR_BMPHANDLER_H

#include <stdio.h>
#include "Image.h"
struct BMP_Header {
    char signature[2];
    int size;
    short reserved1;
    short reserved2;
    int offset_pixel_array;
};
struct DIB_Header{
    int header_size;
    int width;
    int height;
    short planes;
    short bit_per_pixel;
    int compression;
    int image_size;
    int x_pixels_per_meter;
    int y_pixels_per_meter;
    int colors_in_table;
    int important_color_count;
};
/**
* Read BMP header of a BMP file.
*
* @param file: A pointer to the file being read
* @param header: Pointer to the destination BMP header
*/
void readBMPHeader(FILE* file, struct BMP_Header* header);
/**
* Write BMP header of a file. Useful for creating a BMP file.
*
* @param file: A pointer to the file being written
* @param header: The header to write to the file
*/
void writeBMPHeader(FILE* file, struct BMP_Header* header);
/**
* Read DIB header from a BMP file.
*
* @param file: A pointer to the file being read
* @param header: Pointer to the destination DIB header
*/
void readDIBHeader(FILE* file, struct DIB_Header* header);
/**
* Write DIB header of a file. Useful for creating a BMP file.
*
* @param file: A pointer to the file being written
* @param header: The header to write to the file
*/
void writeDIBHeader(FILE* file, struct DIB_Header* header);
/**
* Make BMP header based on width and height. Useful for creating a BMP file.
*
* @param header: Pointer to the destination DIB header
* @param width: Width of the image that this header is for
* @param height: Height of the image that this header is for
*/
void makeBMPHeader(struct BMP_Header* header, int width, int height);
/**
* Make new DIB header based on width and height.Useful for creating a BMP file.
*
* @param header: Pointer to the destination DIB header
* @param width: Width of the image that this header is for
* @param height: Height of the image that this header is for
*/
void makeDIBHeader(struct DIB_Header* header, int width, int height);
/**
* Read Pixels from BMP file based on width and height.
*
* @param file: A pointer to the file being read
* @param pArr: Pixel array to store the pixels being read
* @param width: Width of the pixel array of this image
* @param height: Height of the pixel array of this image
*/
void readPixelsBMP(FILE* file, struct Pixel** pArr, int width, int height);
/**
* Write Pixels from BMP file based on width and height.
*
* @param file: A pointer to the file being read or written
* @param pArr: Pixel array of the image to write to the file
* @param width: Width of the pixel array of this image
* @param height: Height of the pixel array of this image
*/
void writePixelsBMP(FILE* file, struct Pixel** pArr, int width, int height);

#endif //MCCOYIMAGEPROCESSOR_BMPHANDLER_H
