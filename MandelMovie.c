/*********************
**  Mandelbrot fractal movie generator
** clang -Xpreprocessor -fopenmp -lomp -o Mandelbrot Mandelbrot.c
** by Dan Garcia <ddgarcia@cs.berkeley.edu>
** Modified for this class by Justin Yokota and Chenyu Shi
**********************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "ComplexNumber.h"
#include "Mandelbrot.h"
#include "ColorMapInput.h"
#include <sys/types.h>
#include <string.h>

void printUsage(char *argv[])
{
	printf("Usage: %s <threshold> <maxiterations> <center_real> <center_imaginary> <initialscale> <finalscale> <framecount> <resolution> <output_folder> <colorfile>\n", argv[0]);
	printf("    This program simulates the Mandelbrot Fractal, and creates an iteration map of the given center, scale, and resolution, then saves it in output_file\n");
}

/*
This function calculates the threshold values of every spot on a sequence of frames. The center stays the same throughout the zoom. First frame is at initialscale, and last frame is at finalscale scale.
The remaining frames form a geometric sequence of scales, so
if initialscale=1024, finalscale=1, framecount=11, then your frames will have scales of 1024, 512, 256, 128, 64, 32, 16, 8, 4, 2, 1.
As another example, if initialscale=10, finalscale=0.01, framecount=5, then your frames will have scale 10, 10 * (0.01/10)^(1/4), 10 * (0.01/10)^(2/4), 10 * (0.01/10)^(3/4), 0.01 .
*/
void MandelMovie(double threshold, u_int64_t max_iterations, ComplexNumber *center, double initialscale, double finalscale, int framecount, u_int64_t resolution, u_int64_t **output)
{
	// YOUR CODE HERE
	double scale;
	for (int i = 0; i < framecount; i++)
	{
		scale = initialscale * pow(pow(finalscale / initialscale, 1.0 / (framecount - 1)), i);
		Mandelbrot(threshold, max_iterations, center, scale, resolution, *(output + i));
	}
}

/**************
**This main function converts command line inputs into the format needed to run MandelMovie.
**It then uses the color array from FileToColorMap to create PPM images for each frame, and stores it in output_folder
***************/
int main(int argc, char *argv[])
{
	// Tips on how to get started on main function:
	// MandelFrame also follows a similar sequence of steps; it may be useful to reference that.
	// Mayke you complete the steps below in order.

	// STEP 1: Convert command line inputs to local variables, and ensure that inputs are valid.
	/*
	Check the spec for examples of invalid inputs.
	Remember to use your solution to B.1.1 to process colorfile.
	*/

	// YOUR CODE HERE
	//  argv 1 double threshold, 2 u_int64_t max_iterations, 3 center_real, 4 center_imaginary, 5 double initialscale, 6 double finalscale, 7 int framecount, 8 u_int64_t resolution,
	//  9 outputfolder, 10 colorfile
	if (argc != 11)
	{
		return 1;
	}
	double threshold = atof(argv[1]);
	u_int64_t max_iterations = (u_int64_t)atoi(argv[2]);
	double center_real = atof(argv[3]);
	double center_imaginery = atof(argv[4]);
	double initialscale = atof(argv[5]);
	double finalscale = atof(argv[6]);
	int framecount = atoi(argv[7]);
	u_int64_t resolution = (u_int64_t)atoi(argv[8]);
	char *outpoutfolder = argv[9];
	char *colorfile = argv[10];

	if (threshold <= 0 || max_iterations <= 0 || initialscale <= 0 || finalscale <= 0 || framecount <= 0 || framecount > 10000 || resolution < 0 || (framecount == 1 && fabs(initialscale - finalscale) < 1e-12))
	{
		return 1;
	}

	// STEP 2: Run MandelMovie on the correct arguments.
	/*
	MandelMovie requires an output array, so make sure you allocate the proper amount of space.
	If allocation fails, free all the space you have already allocated (including colormap), then return with exit code 1.
	*/
	ComplexNumber *center = newComplexNumber(center_real, center_imaginery);
	// allocate the array of pointers
	u_int64_t **frames = (u_int64_t **)malloc(sizeof(u_int64_t *) * framecount);
	if (frames == NULL)
	{
		return 1;
	}
	// allocate an array of n*n length, which is pointed by a pointer in the above array of pointers
	u_int64_t n = resolution * 2 + 1;
	for (int i = 0; i < framecount; i++)
	{
		*(frames + i) = (u_int64_t *)malloc(sizeof(u_int64_t) * n * n);
		if (*(frames + i) == NULL)
		{
			return 1;
		}
	}
	MandelMovie(threshold, max_iterations, center, initialscale, finalscale, framecount, resolution, frames);

	// YOUR CODE HERE

	// STEP 3: Output the results of MandelMovie to .ppm files.
	/*
	Convert from iteration count to colors, and output the results into output files.
	Use what we showed you in Part B.1.2, create a seqeunce of ppm files in the output folder.
	Feel free to create your own helper function to complete this step.
	As a reminder, we are using P6 format, not P3.
	*/
	int *colorcount = (int *)malloc(sizeof(int));
	uint8_t **colormap = FileToColorMap(colorfile, colorcount);
	if (colormap == NULL)
	{
		return 1;
	}
	FILE *outputfile;
	int filenamelength = strlen(outpoutfolder) + strlen("/frame00000.ppm") + 1;
	char buffer[filenamelength];
	uint8_t black[3] = {0, 0, 0};
	for (int i = 0; i < framecount; i++)
	{
		u_int64_t *mandelbrotplot = *(frames + i);
		sprintf(buffer, "%s/frame%05d.ppm", outpoutfolder, i);
		outputfile = fopen(buffer, "w+");
		if (outputfile == NULL)
		{
			return 1;
		}
		fprintf(outputfile, "P6 %lu %lu 255\n", n, n);
		int colorindex;
		for (int j = 0; j < n; j++)
			for (int k = 0; k < n; k++)
			{
				u_int64_t iterations = *(mandelbrotplot + j * n + k);
				if (iterations == 0)
				{
					fwrite(black, 1, 3, outputfile);
				}
				else
				{
					colorindex = (iterations - 1) % *colorcount;
					fwrite(*(colormap + colorindex), 1, 3, outputfile);
				}
			}
		fclose(outputfile);
	}

	// YOUR CODE HERE

	// STEP 4: Free all allocated memory
	/*
	Make sure there's no memory leak.
	*/
	// YOUR CODE HERE
	free(center);
	for (int i = 0; i < (*colorcount); i++)
	{
		free(*(colormap + i));
	}
	free(colorcount);
	free(colormap);
	for (int i = 0; i < framecount; i++)
	{
		free(*(frames + i));
	}
	free(frames);

	return 0;
}
