#ifndef VECTOR_H
#define VECTOR_H

// Include standard libraries
#include <stdlib.h>
#include <math.h>

// Define types to be used in c file
typedef double* vector3_t;


/**
 * Add two vectors element-wise.
 * 
 * @param c  output vector
 * @param a  first input vector
 * @param b  second input vector
 */
void vector3_add(vector3_t c, vector3_t a, vector3_t b);

/**
 * Subtract two vectors element-wise.
 * 
 * @param c  output vector
 * @param a  input vector to subtract from
 * @param b  input vector to use for subtraction
 */
void vector3_sub(vector3_t c, vector3_t a, vector3_t b);

/**
 * Scale a vector.
 * 
 * @param c  output vector
 * @param a  vector to scale
 * @param b  scalar value
 */
void vector3_scale(vector3_t c, vector3_t a, double b);

/**
 * Dot product two vectors.
 * 
 * @param  a  first input vector
 * @param  b  second input vector
 * @return    result of dot product
 */
double vector3_dot(vector3_t a, vector3_t b);

/**
 * Cross product two vectors.
 * 
 * @param c  output vector
 * @param a  first input vector
 * @param b  second input vector
 */
void vector3_cross(vector3_t c, vector3_t a, vector3_t b);

/**
 * Create a new vector.
 * 
 * @param  a  x value of vector
 * @param  b  y value of vector
 * @param  c  z value of vector
 * @return    newly created vector
 */
vector3_t vector3_create(double a, double b, double c);

/**
 * Create a new unit vector.
 * 
 * @param  a  x value of vector
 * @param  b  y value of vector
 * @param  c  z value of vector
 * @return    newly created vector
 */
vector3_t vector3_createUnit(double a, double b, double c);

#endif  // VECTOR_H
