// Include header file
#include "helpers.h"


double radialAttenuation(light_t *light, double distance) {
  if (light->light_kind == LIGHT_KIND_SPOT || distance == INFINITY) {
    return 1.0;
  }
  else {
    return 1 / (light->radial_a0 +
                light->radial_a1 * distance +
                light->radial_a2 * pow(distance, 2));
  }
}


double angularAttenuation(light_t *light, vector3_t objDirection) {
  if (light->light_kind != LIGHT_KIND_SPOT) {
    return 1.0;
  }

  double dot = vector3_dot(objDirection, light->direction);

  if (acos(dot) * 180.0 / M_PI > light->theta) {
    return 0.0;
  }
  else {
    return pow(dot, light->angular_a0);
  }
}


double sphereIntersection(vector3_t direction, sphere_t* sphere) {

  vector3_t position = sphere->position;

  // Calculate variables to use in quadratic formula
  double b = -2*(position[0]*direction[0] +
                 position[1]*direction[1] +
                 position[2]*direction[2]);

  double c = pow(position[0], 2) +
             pow(position[1], 2) +
             pow(position[2], 2) -
             pow(sphere->radius, 2);

  double discr = pow(b, 2) - 4*c;

  if (discr < 0) {
    return NO_INTERSECTION_FOUND;
  }
  else {

    // Prioritize closest intersection
    double t1 = (-b - sqrt(discr)) / 2;
    if (t1 > 0) {
      return t1;
    }

    double t2 = (-b + sqrt(discr)) / 2;
    if (t2 > 0) {
      return t2;
    }

    return NO_INTERSECTION_FOUND;
  }
}


double planeIntersection(vector3_t direction, plane_t* plane) {

  // No intersections if the vector is parallel to the plane
  double product = vector3_dot(direction, plane->normal);
  if (product == 0) {
    return NO_INTERSECTION_FOUND;
  }
  
  // Calculate the t scalar of intersection
  vector3_t subVector = vector3_sub(plane->position, direction);
  double t = vector3_dot(subVector, plane->normal) / product;

  // Only return t when it is a positive scalar
  if (t > 0) {
    return t;
  }
  else return NO_INTERSECTION_FOUND;
}
