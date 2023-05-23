/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

// number of neighbors
const size_t number_of_neighbors = 8;

// offset of all neighbors
const int offset_of_neighbors[8][2] = {
	{-1, -1},
	{-1, 0},
	{-1, 1},
	{0, -1},
	{0, 1},
	{1, -1},
	{1, 0},
	{1, 1}
};

static uint32_t get_bit(uint32_t rule, uint32_t n) {
	return (rule >> n) & 1;
}

static uint32_t moveByOffset(uint32_t start, uint32_t size, int offset) {
	return (start + offset + size) % size;
}

static uint32_t isAlive(Image* image, int row, int col) {
	if (image->image[row][col].R == 255 
		&& image->image[row][col].G == 255
		&& image->image[row][col].B == 255) {
		return 1;
	}
	return 0;
}

static uint32_t getNumberOfNeighborsAlive(Image* image, int row, int col) {
	uint32_t cnt = 0;
	for (size_t i = 0; i < number_of_neighbors; ++i) {
		int new_row = moveByOffset(row, image->rows, offset_of_neighbors[i][0]);
		int new_col = moveByOffset(col, image->cols, offset_of_neighbors[i][1]);
		if (isAlive(image, new_row, new_col) == 1) {
			++cnt;
		}
	}
	return cnt;
}

//Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
//Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
//and the left column as adjacent to the right column.
Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule)
{
	//YOUR CODE HERE
	Color* color = (Color*)malloc(sizeof(Color));
	if (color == NULL) {
		return NULL;
	}

	uint32_t nums_of_neigh_alive = getNumberOfNeighborsAlive(image, row, col);

	// if image->image[row][col] is alive
	if (isAlive(image, row, col) == 1) {
		rule >>= 9;
	}
	uint32_t next_state = get_bit(rule, nums_of_neigh_alive);
	if (next_state == 1) {
		color->R = 255;
		color->G = 255;
		color->B = 255;
	}
	else {
		color->R = 0;
		color->G = 0;
		color->B = 0;
	}

	return color;
}

//The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
//You should be able to copy most of this from steganography.c
Image *life(Image *image, uint32_t rule)
{
	//YOUR CODE HERE
	Image* new_image = (Image*)malloc(sizeof(Image));
	if (new_image == NULL) {
		return NULL;
	}
	new_image->rows = image->rows;
	new_image->cols = image->cols;
	new_image->image = (Color**)calloc(new_image->rows, sizeof(Color*));
	if (new_image->image == NULL) {
		return NULL;
	}

	for (size_t i = 0; i < new_image->rows; ++i) {
		new_image->image[i] = (Color*)calloc(new_image->cols, sizeof(Color));
		if (new_image->image[i] == NULL) {
			return NULL;
		}
	}

	for (size_t i = 0; i < new_image->rows; ++i) {
		for (size_t j = 0; j < new_image->cols; ++j) {
			Color* tmp = evaluateOneCell(image, i, j, rule);
			new_image->image[i][j].R = tmp->R;
			new_image->image[i][j].G = tmp->G;
			new_image->image[i][j].B = tmp->B;
			free(tmp);
		}
	}
	return new_image;
}

/*
Loads a .ppm from a file, computes the next iteration of the game of life, then prints to stdout the new image.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
argv[2] should contain a hexadecimal number (such as 0x1808). Note that this will be a string.
You may find the function strtol useful for this conversion.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!

You may find it useful to copy the code from steganography.c, to start.
*/
int main(int argc, char **argv)
{
	//YOUR CODE HERE
	if (argc != 3) {
		printf("usage: %s filename rule\n", argv[0]);
		printf("filename is an ASCII PPM file (type P3) with maximum value 255.\n");
		printf("rule is a hex number beginning with 0x; Life is 0x1808.");
		return -1;
	}
	uint32_t rule = (uint32_t)strtol(argv[2], NULL, 16);
	Image* image = readData(argv[1]);
	if (image == NULL) return -1;
	Image* new_image = life(image, rule);
	if (new_image == NULL) return -1;
	writeData(new_image);
	freeImage(image);
	freeImage(new_image);
	return 0;
}
