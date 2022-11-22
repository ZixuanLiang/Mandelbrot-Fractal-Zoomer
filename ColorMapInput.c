/*********************
**  Color Map generator
** Skeleton by Justin Yokota
**********************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include "ColorMapInput.h"

/**************
**This function reads in a file name colorfile.
**It then uses the information in colorfile to create a color array, with each color represented by an int[3].
***************/
uint8_t **FileToColorMap(char *colorfile, int *colorcount)
{
	// YOUR CODE HERE
	FILE* file = fopen(colorfile, "r");  
	fscanf(file, "%d", colorcount);
	// address of the first element of the array of pointers 
	uint8_t **colorMap = (uint8_t **)malloc(*colorcount * sizeof(uint8_t*));
	int i;
	for (i = 0; i < *colorcount; i++)
	{
		*(colorMap + i) = (uint8_t*)malloc(sizeof(uint8_t) * 3);
		fscanf(file, "%hhu %hhu %hhu", *(colorMap + i), *(colorMap + i) + 1, *(colorMap + i) + 2);
	}
	fclose(file);
	if (i != *colorcount) {
		free(colorMap);
		return NULL;
	} else {
		return colorMap;
	}
}
