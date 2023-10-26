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

    free(*img);
    *img = NULL;
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
    //Traverse the pixel array in order to change value of each pixel
    for (int i = 0; i < img->height; i++) {
        for (int j = 0; j < img->width; j++) {
            double lum;
            //Find the gray scale value based on luminosity equation
            lum =  (img->pArr[i][j].b*0.114) + (img->pArr[i][j].r*0.299) + (img->pArr[i][j].g*0.587);
            //Set each pixel to the same val found through lum eq
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
    //Highest value a rgb value can be is 255
    unsigned char highestValue = 255;
    unsigned char lowestValue = 0;
    //Traverse through the pixel array shifting based on given values
    for (int i = 0; i < img->height; i++) {
        for (int j = 0; j < img->width; j++) {
            //If value given is higher or lower than the highest/lowest value clap to the highest/lowest value
            if(img->pArr[i][j].b  + bShift > highestValue){
                img->pArr[i][j].b = highestValue;
            }
            else if(img->pArr[i][j].b + bShift < lowestValue){
                img->pArr[i][j].b = lowestValue;
            }
            else{
                //Shift based on given value
                img->pArr[i][j].b = img->pArr[i][j].b + bShift;
                }
            if(img->pArr[i][j].r  +  rShift > highestValue){
                img->pArr[i][j].r =  highestValue;
            }
            else if(img->pArr[i][j].r  + rShift < lowestValue){
                img->pArr[i][j].r = lowestValue;
            }
            else{
                img->pArr[i][j].r = img->pArr[i][j].r +  rShift;
                }
            if(img->pArr[i][j].g +  gShift > highestValue){
                img->pArr[i][j].g =  highestValue;
            }
            else if(img->pArr[i][j].g +  gShift < lowestValue){
                img->pArr[i][j].g =  lowestValue;
            }
            else{
                img->pArr[i][j].g = img->pArr[i][j].g + gShift;}
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

    //Find the new height and width based on resize factor
    int newWidth = (int)((float)img->width * factor);
    int newHeight = (int)((float)img->height * factor);

    //Allocate memory for a new temp pixel array to copy values corresponding pixels to
    struct Pixel** new_pixels = (struct Pixel**)malloc(sizeof(struct Pixel*) * newHeight);
    for (int p = 0; p < newWidth; p++) {
        new_pixels[p] = (struct Pixel*)malloc(sizeof(struct Pixel) * newWidth);
    }

    //Traverse through new array and copy corresponding value form old array into the new one
    //Pixel at the midpoint of new array should be the same as the pixel in the midpoint of the old one
    for (int i = 0; i < newHeight; i++) {
        for (int j = 0; j < newWidth; j++) {

            new_pixels[i][j] = img->pArr[(int)((float)i/factor)][(int)((float)j/factor)]; //Just divide
        }
    }

    //Free memory from the former pixel array
    for (int i = 0; i < img->height; i++) {
        free(img->pArr[i]);
    }

    free(img->pArr);
    //Update image with new info
    img->height = newHeight;
    img->width = newWidth;
    //Set image array to new array.
    img->pArr = new_pixels;


}