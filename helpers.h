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


double radialAttenuation(light_t *light, double distance);

double angularAttenuation(light_t *light, vector3_t objDirection);

void diffuseReflection(vector3_t outColor, vector3_t objColor,
                       vector3_t lightColor, vector3_t normal,
                       vector3_t lDirection);

void specularReflection(vector3_t outColor, vector3_t objColor,
                        vector3_t lightColor, vector3_t direction,
                        vector3_t reflection, double shininess);

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
