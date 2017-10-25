#ifndef PPMRW_H
#define PPMRW_H

// Error code constants
#define NO_STRING_FOUND -1
#define MALFORMED_HEADER -2

// Numeric constants
#define STRING_MAX_BUFFER 32

// Include standard libraries
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h> // isspace

// Define types to be used in ppmrw.c 
typedef struct pixel_t pixel_t;
typedef struct ppm_t ppm_t;

struct pixel_t {
  unsigned char r, g, b;
};

struct ppm_t {
  int width;
  int height;
  int maxColorValue;
  pixel_t* pixels;
};

/**
 * Helper method used to get the next series of ASCII 
 * characters from a file handle while ignoring leading whitespace
 * 
 * @param  output  character pointer to memory to save output
 * @param  file    file to be parsed for next string
 * @return         success status of function, NO_STRING_FOUND on error
 */
int getNextString(char *output, FILE *file);

/**
 * Reads in a file handle and outputs a PPMImage structure with
 * the appropriate meta and pixel data stored within
 *
 * @param  output  pointer to a ppm_t structure with the image data
 * @param  file    file to be read into memory
 * @return         success status of function, MALFORMED_HEADER on header error
 */
int readPPM(ppm_t *output, FILE *file);

/**
 * Writes the image data from the PPMImage structure to the given file
 * handle, including appropriate meta data
 *
 * @param  image      pointer to a ppm_t structure with the image data
 * @param  file       file to be written to
 * @param  newFormat  the PPM format to write the file
 * @return            success status of function
 */
int writePPM(ppm_t *image, FILE *file, int newFormat);

#endif  // PPMRW_H
