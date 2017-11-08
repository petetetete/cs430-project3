#ifndef RAYCAST_H
#define RAYCAST_H

// Include standard libraries
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "ppmrw.h"
#include "vector.h"
#include "parsing.h"
#include "helpers.h"

// Numeric constants
#define PPM_OUTPUT_VERSION 3
#define MAX_SCENE_OBJECTS 128
#define MAX_SCENE_LIGHTS 128
#define FOCAL_LENGTH 1.0 // In world units
#define EPSILON_OFFSET 0.000125

// String constants
#define USAGE_MESSAGE "\
Usage: raycast width height input_file output.ppm\n\
  width: pixel width of the view plane\n\
  height: pixel height of the view plane\n\
  input_file: csv file of scene objects\n\
  output_file: final out PPM file name\n"


/**
 * Raycast primitive used to send a ray and determine if an
 * object was hit and the t intersection location of it
 * 
 * @param  outObject   reference to object that was hit
 * @param  origin      point to send the ray from
 * @param  direction   direction to send the ray
 * @param  scene       list of all objects in scene
 * @param  numObjects  number of objects in the scene
 * @return             the t value of the intersection point
 */
double rayObjectIntersect(object_t **outObject, vector3_t origin,
                          vector3_t direction, object_t **scene,
                          int numObjects);

/**
 * Casts a single ray given a particular scene and direction vector,
 * and returns the color of the closest object intersected.
 * 
 * @param  origin      point at which the ray is being sent from
 * @param  direction   vector describing currently cast ray
 * @param  scene       array of objects describing the world
 * @param  numObjects  number of objects in the world
 * @param  lights      array of light objects in the world
 * @param  numLights   number of lights in the world
 * @return             color vector of closest object intersected
 */
vector3_t raycast(vector3_t origin, vector3_t direction,
				  object_t **scene, int numObjects,
				  object_t **lights, int numLights);

/**
 * Renders a PPM image given a particular scene and camera position.
 * 
 * @param  ppmImage    pointer to final output PPM image
 * @param  camera      the camera through which the scene is viewed
 * @param  scene       array of objects describing the world
 * @param  numObjects  number of objects in the world
 * @param  lights      array of light objects in the world
 * @param  numLights   number of lights in the world
 * @return             error status of image rendering
 */
int renderImage(ppm_t *ppmImage, camera_t *camera,
                object_t **scene, int numObjects,
                object_t **lights, int numLights);

#endif  // RAYCAST_H
