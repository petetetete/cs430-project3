#ifndef HELPERS_H
#define HELPERS_H

// Include standard libraries
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "vector.h"
#include "parsing.h"

// Error code constants
#define NO_INTERSECTION_FOUND -2

// Numeric constants
#define M_PI 3.14159265358979323846


/**
 * Calculate radial attenuation at particular distance
 * 
 * @param  light     light to calculate attenuation with
 * @param  distance  distance from the light to the object
 * @return           radial attenuation factor (0-1)
 */
double radialAttenuation(light_t *light, double distance);

/**
 * Calculate angular attenuation of spot lights
 * 
 * @param  light        light to calculate attenuation with
 * @param  loDirection  the direction vector from light to object
 * @return              angular attenuation factor (0-1)
 */
double angularAttenuation(light_t *light, vector3_t loDirection);

/**
 * Calculate diffuse reflection value of the point on an object
 * 
 * @param outColor     output vector representing color
 * @param objColor     current object color
 * @param lightColor   ambient color of the light
 * @param normal       normal of the object
 * @param olDirection  direction vector from object to light
 */
void diffuseReflection(vector3_t outColor, vector3_t objColor,
                       vector3_t lightColor, vector3_t normal,
                       vector3_t olDirection);

/**
 * Calculate specular reflection value of the point on an object
 * 
 * @param outColor    output vector representing color
 * @param objColor    current object color
 * @param lightColor  ambient color of the light
 * @param direction   vector from object to camera
 * @param reflection  reflection of the normal vector
 * @param shininess   shininess of particular object
 */
void specularReflection(vector3_t outColor, vector3_t objColor,
                        vector3_t lightColor, vector3_t direction,
                        vector3_t reflection, double shininess);

/**
 * Clamp value to between a given range of values
 * 
 * @param  value  value to be clamped
 * @param  min    maximum valid value
 * @param  max    minimum valid value
 * @return        value after being clamped
 */
double clampValue(double value, double min, double max);

/**
 * Returns scalar t value of intersection between a direction
 * vector and a sphere, described by a origin point, and a radius.
 * 
 * @param  origin     the origin point of the vector
 * @param  direction  the vector to check for intersection
 * @param  sphere     the sphere that may be intersected
 * @return            scalar value to apply to vector to find intersection
 */
double sphereIntersect(vector3_t origin, vector3_t direction, sphere_t *sphere);

/**
 * Returns scalar t value of intersection between a direction
 * vector and a plane, described by its origin and normal vector.
 * 
 * @param  origin     the origin point of the vector
 * @param  direction  the vector to check for intersection
 * @param  plane      the plane that may be intersected
 * @return            scalar value to apply to vector to find intersection
 */
double planeIntersect(vector3_t origin, vector3_t direction, plane_t *plane);

#endif  // HELPERS_H
