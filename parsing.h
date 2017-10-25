#ifndef PARSING_H
#define PARSING_H

// Include standard libraries
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "vector.h"

// Define constants
#define OBJECT_KIND_CAMERA 1
#define OBJECT_KIND_SPHERE 2
#define OBJECT_KIND_PLANE 3

// Error code constants
#define INVALID_PARSE_LINE -4
#define NO_CAMERA_FOUND -5

// Numeric constants
#define MAX_LINE_LENGTH 256

// Define types to be used in c file
typedef struct object_t object_t;
typedef struct camera_t camera_t;
typedef struct sphere_t sphere_t;
typedef struct plane_t plane_t;

struct object_t {
  int kind;
};

struct camera_t {
  struct object_t;
  double width;
  double height;
};

struct sphere_t {
  struct object_t;
  vector3_t color;
  vector3_t position;
  double radius;
};

struct plane_t {
  struct object_t;
  vector3_t color;
  vector3_t position;
  vector3_t normal;
};


/**
 * Helper function used to parse camera properties from string.
 * 
 * @param  camera  pointer to output camera
 * @param  line    string containing camera data to parse
 * @return         error status of parsing
 */
int parseCamera(camera_t *camera, char *line);

/**
 * Helper function used to parse sphere properties from string.
 * 
 * @param  sphere  pointer to output sphere
 * @param  line    string containing sphere data to parse
 * @return         error status of parsing
 */
int parseSphere(sphere_t *sphere, char *line);

/**
 * Helper function used to parse plane properties from string.
 * 
 * @param  plane  pointer to output plane
 * @param  line   string containing plane data to parse
 * @return        error status of parsing
 */
int parsePlane(plane_t *plane, char *line);

/**
 * Parse CSV file in to an object array describing the world scene.
 * 
 * @param  camera  pointer to output camera
 * @param  scene   array of objects describing the world
 * @param  file    CSV file to parse for object data
 * @return         error status of parsing
 */
int parseInput(camera_t *camera, object_t **scene, FILE *file);

#endif  // PARSING_H
