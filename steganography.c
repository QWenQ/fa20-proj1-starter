/************************************************************************
**
** NAME:        steganography.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**				Justin Yokota - Starter Code
**				YOUR NAME HERE
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

// 0 <= n < 8
static int get_bit(uint8_t num, int n) {
	return (num >> n) & 1;
}

//Determines what color the cell at the given row/col should be. This should not affect Image, and should allocate space for a new Color.
Color *evaluateOnePixel(Image *image, int row, int col)
{
	//YOUR CODE HERE
	Color* color = (Color*)malloc(sizeof(Color));
	if (color == NULL) {
		return NULL;
	}
	uint8_t blue = image->image[row][col].B;
	int lsb = get_bit(blue, 0);

	if (lsb == 0) {
		color->R = 0;
		color->G = 0;
		color->B = 0;
	}
	else {
		color->R = 255;
		color->G = 255;
		color->B = 255;
	}
	return color;
}

//Given an image, creates a new image extracting the LSB of the B channel.
Image *steganography(Image *image)
{
	//YOUR CODE HERE
	Image* ret = (Image*)malloc(sizeof(Image));
	if (ret == NULL) {
		return NULL;
	}	
	ret->rows = image->rows;
	ret->cols = image->cols;
	ret->image = (Color**)calloc(image->rows, sizeof(Color*));
	if (ret->image == NULL) {
		return NULL;
	}
	for (uint32_t i = 0; i < image->rows; ++i) {
		ret->image[i] = (Color*)calloc(image->cols, sizeof(Color));
		if (ret->image[i] == NULL) {
			return NULL;
		}
		for (uint32_t j = 0; j < image->cols; ++j) {
			Color* tmp = evaluateOnePixel(image, i, j);
			ret->image[i][j].R = tmp->R;
			ret->image[i][j].G = tmp->G;
			ret->image[i][j].B = tmp->B;
			free(tmp);
		}
	}

	return ret;
}

/*
Loads a file of ppm P3 format from a file, and prints to stdout (e.g. with printf) a new image, 
where each pixel is black if the LSB of the B channel is 0, 
and white if the LSB of the B channel is 1.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a file of ppm P3 format (not necessarily with .ppm file extension).
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!
*/
int main(int argc, char **argv)
{
	//YOUR CODE HERE
	if (argc != 2) {
		return -1;
	}
	Image* original_image = readData(argv[1]);
	if (original_image == NULL) {
		return -1;
	}
	Image* new_image = steganography(original_image);
	if (new_image == NULL) {
		return -1;
	}
	writeData(new_image);
	freeImage(original_image);
	freeImage(new_image);
	return 0;
}
