// Include header file
#include "helpers.h"


double radialAttenuation(light_t *light, double distance) {
  return 1 / (light->radial_a0 +
              light->radial_a1 * distance +
              light->radial_a2 * pow(distance, 2));
}


double angularAttenuation(light_t *light, vector3_t loDirection) {

  vector3_t tempOlDirection = vector3_create(0, 0, 0);
  vector3_scale(tempOlDirection, loDirection, -1);

  if (light->light_kind != LIGHT_KIND_SPOT) {
    return 1.0;
  }

  double dot = vector3_dot(tempOlDirection, light->direction);
  free(tempOlDirection);
  
  if (acos(dot) * 180.0 / M_PI > light->theta) {
    return 0.0;
  }
  else {
    return pow(dot, light->angular_a0);
  }
}


void diffuseReflection(vector3_t outColor, vector3_t objColor,
                       vector3_t lightColor, vector3_t normal,
                       vector3_t olDirection) {

  double product = vector3_dot(normal, olDirection);

  if (product > 0) {
    outColor[0] = objColor[0]*lightColor[0]*product;
    outColor[1] = objColor[1]*lightColor[1]*product;
    outColor[2] = objColor[2]*lightColor[2]*product;
  }
  else {
    outColor[0] = 0;
    outColor[1] = 0;
    outColor[2] = 0;
  }
}

void specularReflection(vector3_t outColor, vector3_t objColor,
                        vector3_t lightColor, vector3_t ovDirection,
                        vector3_t reflection, double shininess) {

  double product = vector3_dot(ovDirection, reflection);

  if (product > 0) {
    product = pow(product, shininess);
    outColor[0] = objColor[0]*lightColor[0]*pow(product, shininess);
    outColor[1] = objColor[1]*lightColor[1]*pow(product, shininess);
    outColor[2] = objColor[2]*lightColor[2]*pow(product, shininess);
  }
  else {
    outColor[0] = 0;
    outColor[1] = 0;
    outColor[2] = 0;
  }
}


double clampValue(double value, double min, double max) {
  if (value > max) {
    return max;
  } else if (value < min) {
    return min;
  } else {
    return value;
  }
}


double sphereIntersect(vector3_t origin, vector3_t direction,
                       sphere_t *sphere) {

  vector3_t position = sphere->position;

  // Calculate variables to use in quadratic formula
  double a = pow(direction[0], 2) +
             pow(direction[1], 2) +
             pow(direction[2], 2);

  double b = 2*(direction[0]*(origin[0] - position[0]) +
                direction[1]*(origin[1] - position[1]) +
                direction[2]*(origin[2] - position[2]));

  double c = pow(position[0], 2) + pow(position[1], 2) + pow(position[2], 2) +
             pow(origin[0], 2) + pow(origin[1], 2) + pow(origin[2], 2) +
             -2*(position[0]*origin[0] +
                 position[1]*origin[1] +
                 position[2]*origin[2]) -
             pow(sphere->radius, 2);

  double discr = pow(b, 2) - 4*a*c;

  if (discr < 0) {
    return NO_INTERSECTION_FOUND;
  }
  else {

    // Prioritize closest intersection
    double t1 = (-b - sqrt(discr)) / (2*a);
    if (t1 > 0) {
      return t1;
    }

    double t2 = (-b + sqrt(discr)) / (2*a);
    if (t2 > 0) {
      return t2;
    }

    return NO_INTERSECTION_FOUND;
  }
}


double planeIntersect(vector3_t origin, vector3_t direction, plane_t *plane) {

  // No intersections if the vector is parallel to the plane
  double product = vector3_dot(direction, plane->normal);
  if (product == 0) {
    return NO_INTERSECTION_FOUND;
  }
  
  // Calculate the t scalar of intersection
  vector3_t subVector = vector3_create(0, 0, 0);
  vector3_sub(subVector, plane->position, origin);

  double t = vector3_dot(subVector, plane->normal) / product;

  free(subVector);

  // Only return t when it is a positive scalar
  if (t > 0) {
    return t;
  }
  else return NO_INTERSECTION_FOUND;
}
