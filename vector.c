// Include header file
#include "vector.h"


void vector3_add(vector3_t c, vector3_t a, vector3_t b) {
  c[0] = a[0] + b[0];
  c[1] = a[1] + b[1];
  c[2] = a[2] + b[2];
}


void vector3_sub(vector3_t c, vector3_t a, vector3_t b) {
  c[0] = a[0] - b[0];
  c[1] = a[1] - b[1];
  c[2] = a[2] - b[2];
}


void vector3_scale(vector3_t c, vector3_t a, double b) {
  c[0] = a[0] * b;
  c[1] = a[1] * b;
  c[2] = a[2] * b;
}


double vector3_dot(vector3_t a, vector3_t b) {
  return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
}


void vector3_cross(vector3_t c, vector3_t a, vector3_t b) {
  c[0] = a[1]*b[2] - a[2]*b[1];
  c[1] = a[2]*b[0] - a[0]*b[2];
  c[2] = a[0]*b[1] - a[1]*b[0];
}


double vector3_mag(vector3_t a) {
  return sqrt(pow(a[0], 2) + pow(a[1], 2) + pow(a[2], 2));
}


void vector3_normalize(vector3_t a) {
  vector3_scale(a, a, 1 / vector3_mag(a));
}


vector3_t vector3_create(double a, double b, double c) {
  vector3_t output = malloc(sizeof(double) * 3);
  output[0] = a;
  output[1] = b;
  output[2] = c;
  return output;
}
