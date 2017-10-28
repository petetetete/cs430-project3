// Include header file
#include "parsing.h"

int parseCamera(camera_t *camera, char *line) {

  // Set object kind
  camera->kind = OBJECT_KIND_CAMERA;

  // Variables to parse in to
  double width = 0;
  double height = 0;

  // Try to find width and height elements in file
  char *widthStart = strstr(line, "width:");
  char *heightStart = strstr(line, "height:");

  if (widthStart == NULL || heightStart == NULL) {
    return INVALID_PARSE_LINE;
  }

  // Increment pointer beyond the initial scan string
  sscanf(widthStart + 6, "%lf,", &width);
  sscanf(heightStart + 7, "%lf,", &height);

  // Catch invalid values
  if (width <= 0 || height <= 0) {
    return INVALID_PARSE_LINE;
  }
  else {

    // Populate camera
    camera->width = width;
    camera->height = height;

    return 0;
  }
}

int parseLight(light_t *light, char *line) {

  // Set object kind
  light->kind = OBJECT_KIND_LIGHT;

  // Variables to parse in to
  vector3_t position = vector3_create(INFINITY, INFINITY, INFINITY); // Shared
  vector3_t color = vector3_create(INFINITY, INFINITY, INFINITY);
  double theta = 0;
  double radialA0 = INFINITY; // Light properties
  double radialA1 = INFINITY;
  double radialA2 = INFINITY;
  double angularA0 = INFINITY; // Spot light properties
  vector3_t direction = vector3_create(INFINITY, INFINITY, INFINITY);

  // Try to find elements in line
  char *positionStart = strstr(line, "position:");
  char *colorStart = strstr(line, "color:");
  char *thetaStart = strstr(line, "theta:");
  char *radialA0Start = strstr(line, "radial-a0:");
  char *radialA1Start = strstr(line, "radial-a1:");
  char *radialA2Start = strstr(line, "radial-a2:");
  char *angularA0Start = strstr(line, "angular-a0:");
  char *directionStart = strstr(line, "direction:");

  // Catch obviously invalid objects
  if (positionStart == NULL || colorStart == NULL) {
    return INVALID_PARSE_LINE;
  }

  // Save global light properties
  sscanf(positionStart + 9, " [%lf , %lf , %lf],",
    &position[0], &position[1], &position[2]);
  sscanf(colorStart + 6, " [%lf , %lf , %lf],",
    &color[0], &color[1], &color[2]);

  // Bail out early if the global light properties are bad
  if (position[0] == INFINITY ||
      position[1] == INFINITY ||
      position[2] == INFINITY ||
      color[0] == INFINITY || color[1] == INFINITY || color[2] == INFINITY) {
    return INVALID_PARSE_LINE;
  }

  // Determine type of light
  if (thetaStart != NULL)
    sscanf(thetaStart + 6, "%lf,", &theta);
  
  // If light is a point light
  if (thetaStart == NULL || theta == 0) {
    light->light_kind = LIGHT_KIND_POINT;

    if (radialA0Start == NULL ||
        radialA1Start == NULL ||
        radialA2Start == NULL) {
      return INVALID_PARSE_LINE;
    }

    // Get point light properties
    sscanf(radialA0Start + 10, "%lf,", &radialA0);
    sscanf(radialA1Start + 10, "%lf,", &radialA1);
    sscanf(radialA2Start + 10, "%lf,", &radialA2);

    // Catch invalid values
    if (radialA0 == INFINITY ||
        radialA1 == INFINITY ||
        radialA2 == INFINITY) {
      return INVALID_PARSE_LINE;
    }
    else {

      // Populate light
      light->position = position;
      light->color = color;
      light->radial_a0 = radialA0;
      light->radial_a1 = radialA1;
      light->radial_a2 = radialA2;

      return 0;
    }
  }

  // If light is a spot light
  else {
    light->light_kind = LIGHT_KIND_SPOT;

    if (angularA0Start == NULL ||
        directionStart == NULL) {
      return INVALID_PARSE_LINE;
    }

    // Get spot light properties
    sscanf(angularA0Start + 11, "%lf,", &angularA0);
    sscanf(directionStart + 10, " [%lf , %lf , %lf],",
      &direction[0], &direction[1], &direction[2]);

    // Catch invalid values
    if (angularA0 == INFINITY ||
        direction[0] == INFINITY ||
        direction[1] == INFINITY ||
        direction[2] == INFINITY) {
      return INVALID_PARSE_LINE;
    }
    else {

      // Populate light
      light->position = position;
      light->color = color;
      light->theta = theta;
      light->angular_a0 = angularA0;
      light->direction = direction;

      return 0;
    }
  }
}

int parseSphere(sphere_t *sphere, char *line) {

  sphere->kind = OBJECT_KIND_SPHERE;

  // Variables to parse in to
  vector3_t diffuseColor = vector3_create(INFINITY, INFINITY, INFINITY);
  vector3_t specularColor = vector3_create(INFINITY, INFINITY, INFINITY);
  vector3_t position = vector3_create(INFINITY, INFINITY, INFINITY);
  double radius = INFINITY;

  // Try to find elements in line
  char *diffuseColorStart = strstr(line, "diffuse_color:");
  char *specularColorStart = strstr(line, "specular_color:");
  char *positionStart = strstr(line, "position:");
  char *radiusStart = strstr(line, "radius:");

  if (diffuseColorStart == NULL || specularColorStart == NULL ||
      positionStart == NULL || radiusStart == NULL) {
    return INVALID_PARSE_LINE;
  }

  // Increment pointer beyond the initial scan string
  sscanf(diffuseColorStart + 14, " [%lf , %lf , %lf],",
    &diffuseColor[0], &diffuseColor[1], &diffuseColor[2]);
  sscanf(specularColorStart + 15, " [%lf , %lf , %lf],",
    &specularColor[0], &specularColor[1], &specularColor[2]);
  sscanf(positionStart + 9, " [%lf , %lf , %lf],",
    &position[0], &position[1], &position[2]);
  sscanf(radiusStart + 7, "%lf,", &radius);

  // Catch invalid values
  if (diffuseColor[0] == INFINITY ||
      diffuseColor[1] == INFINITY ||
      diffuseColor[2] == INFINITY ||
      specularColor[0] == INFINITY ||
      specularColor[1] == INFINITY ||
      specularColor[2] == INFINITY ||
      position[0] == INFINITY ||
      position[1] == INFINITY ||
      position[2] == INFINITY ||
      radius == INFINITY) {
    return INVALID_PARSE_LINE;
  }
  else {

    // Populate sphere
    sphere->diffuse_color = diffuseColor;
    sphere->specular_color = specularColor;
    sphere->position = position;
    sphere->radius = radius;

    return 0;
  }
}

int parsePlane(plane_t *plane, char *line) {

  plane->kind = OBJECT_KIND_PLANE;

  // Variables to parse in to
  vector3_t diffuseColor = vector3_create(INFINITY, INFINITY, INFINITY);
  vector3_t specularColor = vector3_create(INFINITY, INFINITY, INFINITY);
  vector3_t position = vector3_create(INFINITY, INFINITY, INFINITY);
  vector3_t normal = vector3_create(INFINITY, INFINITY, INFINITY);

  // Try to find elements in line
  char *diffuseColorStart = strstr(line, "diffuse_color:");
  char *specularColorStart = strstr(line, "specular_color:");
  char *positionStart = strstr(line, "position:");
  char *normalStart = strstr(line, "normal:");

  if (diffuseColorStart == NULL || positionStart == NULL || normalStart == NULL) {
    return INVALID_PARSE_LINE;
  }

  // Increment pointer beyond the initial scan string
  sscanf(diffuseColorStart + 14, " [%lf , %lf , %lf],",
    &diffuseColor[0], &diffuseColor[1], &diffuseColor[2]);
  sscanf(specularColorStart + 15, " [%lf , %lf , %lf],",
    &specularColor[0], &specularColor[1], &specularColor[2]);
  sscanf(positionStart + 9, " [%lf , %lf , %lf],",
    &position[0], &position[1], &position[2]);
  sscanf(normalStart + 7, " [%lf , %lf , %lf],",
    &normal[0], &normal[1], &normal[2]);

  // Catch invalid values
  if (diffuseColor[0] == INFINITY ||
      diffuseColor[1] == INFINITY ||
      diffuseColor[2] == INFINITY ||
      specularColor[0] == INFINITY ||
      specularColor[1] == INFINITY ||
      specularColor[2] == INFINITY ||
      position[0] == INFINITY ||
      position[1] == INFINITY ||
      position[2] == INFINITY ||
      normal[0] == INFINITY ||
      normal[1] == INFINITY ||
      normal[2] == INFINITY) {
    return INVALID_PARSE_LINE;
  }
  else {

    // Populate sphere
    plane->diffuse_color = diffuseColor;
    plane->specular_color = specularColor;
    plane->position = position;
    plane->normal = normal;

    return 0;
  }
}

// return NULL == error, otherwise return array of numObjects
int *parseInput(camera_t *camera, object_t **scene,
               object_t **lights, FILE *file) {

  // Incrementers
  int *numObjects = malloc(sizeof(int)*2);
  numObjects[0] = 0;
  numObjects[1] = 0;
  int lineNumber = 1;

  int errorStatus = 0;
  int cameraFound = 1; // Default to false
  char line[MAX_LINE_LENGTH];

  while (fgets(line, MAX_LINE_LENGTH, file)) {

    // Get object type
    char objectType[20];
    sscanf(line, " %19[a-zA-Z]", objectType);

    // Determine which parse function to use by objectType
    if (strcmp(objectType, "camera") == 0) {
      errorStatus = parseCamera(camera, line);

      // If no error, reset flag
      if (errorStatus == 0) {
        cameraFound = 0;
      }
    }
    else if (strcmp(objectType, "light") == 0) {
      light_t *light = malloc(sizeof(light_t));
      errorStatus = parseLight(light, line);

      // If no error, save object
      if (errorStatus == 0) {
        lights[numObjects[1]++] = (object_t *) light;
      }
    }

    // Handle scene objects
    else if (strcmp(objectType, "sphere") == 0) {
      sphere_t *sphere = malloc(sizeof(sphere_t));
      errorStatus = parseSphere(sphere, line);

      // If no error, save object
      if (errorStatus == 0) {
        scene[numObjects[0]++] = (object_t *) sphere;
      }
    }
    else if (strcmp(objectType, "plane") == 0) {
      plane_t *plane = malloc(sizeof(plane_t));
      errorStatus = parsePlane(plane, line);

      // If no error, save object
      if (errorStatus == 0) {
        scene[numObjects[0]++] = (object_t *) plane;
      }
    }

    if (errorStatus != 0) {
      fprintf(stdout, "Warning: Invalid object on line %d of CSV\n", lineNumber);
    }

    lineNumber += 1;
  }

  // Ensure that a camera was found
  if (cameraFound != 0) {
    return NULL;
  }
  else return numObjects;
}