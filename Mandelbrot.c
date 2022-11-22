/*********************
**  Mandelbrot fractal
** clang -Xpreprocessor -fopenmp -lomp -o Mandelbrot Mandelbrot.c
** by Dan Garcia <ddgarcia@cs.berkeley.edu>
** Modified for this class by Justin Yokota and Chenyu Shi
**********************/

#include <stdio.h>
#include <stdlib.h>
#include "ComplexNumber.h"
#include "Mandelbrot.h"
#include <sys/types.h>

/*
This function returns the number of iterations before the initial point >= the threshold.
If the threshold is not exceeded after maxiters, the function returns 0.
*/
u_int64_t MandelbrotIterations(u_int64_t maxiters, ComplexNumber * point, double threshold)
{
    //YOUR CODE HERE
    ComplexNumber *m = newComplexNumber(0, 0);
    ComplexNumber *product;
    for (int i = 1; i <= maxiters; i++)
    {
      product = ComplexProduct(m, m);
      free(m);
      m = ComplexSum(product, point);
      free(product);
      if (ComplexAbs(m) >= threshold)
      {
        free(m);
        return i;
      }     
    }
  free(m);
  return 0;
}

/*
This function calculates the Mandelbrot plot and stores the result in output.
The number of pixels in the image is resolution * 2 + 1 in one row/column. It's a square image.
Scale is the the distance between center and the top pixel in one dimension.
*/
void Mandelbrot(double threshold, u_int64_t max_iterations, ComplexNumber* center, double scale, u_int64_t resolution, u_int64_t * output){
    //YOUR CODE HERE
    double initialreal = Re(center) - scale;
    double initialmaginery = Im(center) + scale;
    int numOfCol = 2 * resolution + 1;
    double interval = scale / resolution;
    ComplexNumber *c;
    for (int i = 0; i < numOfCol; i++)//row
    {
      for (int j = 0; j < numOfCol; j++){//col
        c = newComplexNumber(initialreal + j * interval, initialmaginery - interval * i);
        *(output + i * numOfCol + j) = MandelbrotIterations(max_iterations, c, threshold);
        free(c);
      }
    }
    
}


