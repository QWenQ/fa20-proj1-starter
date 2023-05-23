/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-15
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "imageloader.h"

//Opens a .ppm P3 image file, and constructs an Image object. 
//You may find the function fscanf useful.
//Make sure that you close the file with fclose before returning.
Image *readData(char *filename) 
{
	//YOUR CODE HERE
	FILE* fp = fopen(filename, "r");
	if (fp == NULL) {
		return NULL;
	}
	char buf[5];
	uint32_t col = 0;
	uint32_t row = 0;
	int scale = 0;
	fscanf(fp, "%s %d %d %d", buf, &col, &row, &scale);
	if (strncmp(buf, "P3", 2) != 0) {
		return NULL;
	}
	Image* ret = (Image*)malloc(sizeof(Image));
	ret->rows = row;
	ret->cols = col;
	// ret->image = (Color*)calloc(row * col, sizeof(Color));
	ret->image = (Color**)calloc(row, sizeof(Color*));
	if (ret->image == NULL) {
		return NULL;
	}
	for (int i = 0; i < row; ++i) {
		ret->image[i] = (Color*)calloc(col, sizeof(Color));
		if (ret->image[i] == NULL) {
			return NULL;
		}
	}
	for (int i = 0; i < row; ++i) {
		for (int j = 0; j < col; ++j) {
			fscanf(fp, "%hhd %hhd %hhd", &(ret->image[i][j].R), &(ret->image[i][j].G), &(ret->image[i][j].B));
		}
	}
	return ret;
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{
	//YOUR CODE HERE
	printf("P3\n%3d %3d\n255\n", image->cols, image->rows);
	for (int i = 0; i < image->rows; ++i) {
		for (int j = 0; j < image->cols; ++j) {
			printf("%3d %3d %3d", image->image[i][j].R, image->image[i][j].G, image->image[i][j].B);
			if (j != image->cols - 1) {
				printf("   ");
			}
		}
		printf("\n");
	}
}

//Frees an image
void freeImage(Image *image)
{
	//YOUR CODE HERE
	for (int i = 0; i < image->rows; ++i) {
		free(image->image[i]);
	}
	free(image->image);
	free(image);
}
