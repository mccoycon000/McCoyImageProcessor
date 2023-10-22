////////////////////////////////////////////////////////////////////////////////
// Include Files
#include <stdio.h>
#include <stdlib.h>
#include "Image.h"

/* Creates a new image and returns it.
*
* @param pArr: Pixel array of this image.
* @param width: Width of this image.
* @param height: Height of this image.
* @return A pointer to a new image.
*/
Image* image_create(struct Pixel** pArr, int width, int height){

    Image* newImage = (Image*)malloc(sizeof(Image));

    newImage->pArr = pArr;
    newImage->width = width;
    newImage->height = height;

    return newImage;
}
/* Destroys an image. Does not deallocate internal pixel array.
*
* @param img: the image to destroy.
*/
void image_destroy(Image** img){

    free(img);
    img = NULL;
}
/* Returns a double pointer to the pixel array.
*
* @param img: the image.
*/
struct Pixel** image_get_pixels(Image* img){

    return img->pArr;
}
/* Returns the width of the image.
*
* @param img: the image.
*/
int image_get_width(Image* img){

    return img->width;
}
/* Returns the height of the image.
*
* @param img: the image.
*/
int image_get_height(Image* img){

    return img->height;
}
/* Converts the image to grayscale.
*
* @param img: the image.
*/
void image_apply_bw(Image* img){
    for (int i = 0; i < img->height; i++) {
        for (int j = 0; j < img->width; j++) {
            int lum;
            lum =  (img->pArr[i][j].b*0.114) + (img->pArr[i][j].r*0.299) + (img->pArr[i][j].g*0.587);
            img->pArr[i][j].b = (unsigned char) lum;
            img->pArr[i][j].r = (unsigned char) lum;
            img->pArr[i][j].g = (unsigned char) lum;
        }
    }
}
/**
* Shift color of the internal Pixel array. The dimension of the array is width *
height.
* The shift value of RGB is rShift, gShift，bShift. Useful for color shift.
*
* @param img: the image.
* @param rShift: the shift value of color r shift
* @param gShift: the shift value of color g shift
* @param bShift: the shift value of color b shift
*/
void image_apply_colorshift(Image* img, int rShift, int gShift, int bShift) {
    unsigned char rS = rShift;
    unsigned char gS = gShift;
    unsigned char bS = bShift;
    for (int i = 0; i < img->height; i++) {
        for (int j = 0; j < img->width; j++) {
            if(bShift > 0){
                if(img->pArr[i][j].b + bS > 255){
                    img->pArr[i][j].b = 255;
                }else{
                    img->pArr[i][j].b = img->pArr[i][j].b + bS;
                }
            }
            if(rShift > 0){
                if(img->pArr[i][j].r + rS > 255){
                    img->pArr[i][j].r = 255;
                }else{
                    img->pArr[i][j].r = img->pArr[i][j].r + rS;
                }
            }
            if(gShift > 0){
                if(img->pArr[i][j].g + gS > 255){
                    img->pArr[i][j].g = 255;
                }else{
                    img->pArr[i][j].g = img->pArr[i][j].g + bS;
                }
            }
        }
    }
}
/* Converts the image to grayscale. If the scaling factor is less than 1 the new
image will be
* smaller, if it is larger than 1, the new image will be larger.
*
* @param img: the image.
* @param factor: the scaling factor
*/
void image_apply_resize(Image* img, float factor){

    int newWidth = img->width * factor;
    int newHeight = img->height * factor;

    struct Pixel** pixels = (struct Pixel**)malloc(sizeof(struct Pixel*) * newHeight);
    for (int p = 0; p < newWidth; p++) {
        pixels[p] = (struct Pixel*)malloc(sizeof(struct Pixel) * newWidth);
    }


    img->height = newHeight;
    img->width = newWidth;

    for (int i = 0; i < img->height; i++) {
        for (int j = 0; j < img->width; j++) {

            pixels[i][j] = img->pArr[(int)(i/factor)][(int)(j/factor)]; //Just divide
        }
    }


    img->pArr = (struct Pixel**) realloc(img->pArr, (sizeof(struct Pixel*) * newHeight));
    for (int p = 0; p < newWidth; p++) {
        img->pArr[p] = (struct Pixel*)realloc(img->pArr[p],(sizeof(struct Pixel) * newWidth));
    }

    for (int i = 0; i < img->height; i++) {
        for (int j = 0; j < img->width; j++) {
            img->pArr[i][j] = pixels[i][j];
        }
    }

    free(pixels);
    pixels = NULL;



}