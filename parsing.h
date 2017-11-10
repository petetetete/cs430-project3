#ifndef PARSING_H
#define PARSING_H

// Include standard libraries
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "vector.h"

// Define constants
#define OBJECT_KIND_CAMERA 1
#define OBJECT_KIND_LIGHT 2
#define OBJECT_KIND_SPHERE 3
#define OBJECT_KIND_PLANE 4

#define LIGHT_KIND_POINT 1
#define LIGHT_KIND_SPOT 2

// Error code constants
#define INVALID_PARSE_LINE -4

// Numeric constants
#define MAX_LINE_LENGTH 256
#define DEFAULT_SHININESS 20

// Define types to be used in c file
typedef struct object_t object_t;
typedef struct camera_t camera_t;
typedef struct sphere_t sphere_t;
typedef struct plane_t plane_t;
typedef struct light_t light_t;

struct object_t {
  int kind;
};

struct camera_t {
  struct object_t;
  double width;
  double height;
  vector3_t position;
};

struct sphere_t {
  struct object_t;
  vector3_t diffuse_color;
  vector3_t specular_color;
  vector3_t position;
  double radius;
  double shininess;
};

struct plane_t {
  struct object_t;
  vector3_t diffuse_color;
  vector3_t specular_color;
  vector3_t position;
  vector3_t normal;
  double shininess;
};

struct light_t {
  struct object_t;
  vector3_t position;
  vector3_t color;
  double radial_a2;
  double radial_a1;
  double radial_a0;
  double theta;
  double angular_a0;
  vector3_t direction;
  int light_kind;
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
 * Helper function used to parse light properties from string.
 * 
 * @param  light  pointer to output light
 * @param  line   string containing light data to parse
 * @return        error status of parsing
 */
int parseLight(light_t *light, char *line);

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
 * @param  lights  array of light objects in the world
 * @param  file    CSV file to parse for object data
 * @return         number of objects tuple, [objects, lights]
 *                 NULL is an error
 */
int *parseInput(camera_t *camera, object_t **scene,
               object_t **lights, FILE *file);

#endif  // PARSING_H
