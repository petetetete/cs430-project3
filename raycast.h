#ifndef RAYCAST_H
#define RAYCAST_H

// Include standard libraries
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "ppmrw.h"
#include "vector.h"
#include "parsing.h"

// Error code constants
#define NO_INTERSECTION_FOUND -2

// Numeric constants
#define PPM_OUTPUT_VERSION 6
#define MAX_SCENE_OBJECTS 128
#define FOCAL_LENGTH 1.0 // In world units

// String constants
#define USAGE_MESSAGE "\
Usage: raycast width height input_file output.ppm\n\
  width: pixel width of the view plane\n\
  height: pixel height of the view plane\n\
  input_file: csv file of scene objects\n\
  output_file: final out PPM file name\n"

/**
 * Returns scalar t value of intersection between a direction
 * vector and a sphere, described by a origin point, and a radius.
 * 
 * @param  direction  the vector to check for intersection
 * @param  sphere     the sphere that may be intersected
 * @return            scalar value to apply to vector to find intersection
 */
double sphereIntersection(vector3_t direction, sphere_t* sphere);

/**
 * Returns scalar t value of intersection between a direction
 * vector and a plane, described by its origin and normal vector.
 * 
 * @param  direction  the vector to check for intersection
 * @param  plane      the plane that may be intersected
 * @return            scalar value to apply to vector to find intersection
 */
double planeIntersection(vector3_t direction, plane_t* plane);

/**
 * Casts a single ray given a particular scene and direction vector,
 * and returns the color of the closest object intersected.
 * 
 * @param  scene       array of objects describing the world
 * @param  direction   vector describing currently cast ray
 * @param  numObjects  number of objects in the world
 * @return             color vector of closest object intersected
 */
vector3_t raycast(object_t **scene, vector3_t direction, int numObjects);

/**
 * Renders a PPM image given a particular scene and camera position.
 * 
 * @param  ppmImage    pointer to final output PPM image
 * @param  camera      the camera through which the scene is viewed
 * @param  scene       array of objects describing the world
 * @param  numObjects  number of objects in the world
 * @return             error status of image rendering
 */
int renderImage(ppm_t *ppmImage, camera_t *camera,
                object_t **scene, int numObjects);

#endif  // RAYCAST_H
