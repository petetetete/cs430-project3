// Include header file
#include "parsing.h"

int parseCamera(camera_t *camera, char *line) {

  // Set object kind
  camera->kind = OBJECT_KIND_CAMERA;

  // Variables to parse in to
  double width;
  double height;

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
  if (width == 0 || height == 0) {
    return INVALID_PARSE_LINE;
  }
  else {

    // Populate camera
    camera->width = width;
    camera->height = height;

    return 0;
  }
}

int parseSphere(sphere_t *sphere, char *line) {

  sphere->kind = OBJECT_KIND_SPHERE;

  // Variables to parse in to
  vector3_t color = vector3_create(INFINITY, INFINITY, INFINITY);
  vector3_t position = vector3_create(INFINITY, INFINITY, INFINITY);
  double radius = INFINITY;

  // Try to find width and height elements in file
  char *colorStart = strstr(line, "color:");
  char *positionStart = strstr(line, "position:");
  char *radiusStart = strstr(line, "radius:");

  if (colorStart == NULL || positionStart == NULL || radiusStart == NULL) {
    return INVALID_PARSE_LINE;
  }

  // Increment pointer beyond the initial scan string
  sscanf(colorStart + 6, " [%lf , %lf , %lf],",
    &color[0], &color[1], &color[2]);
  sscanf(positionStart + 9, " [%lf , %lf , %lf],",
    &position[0], &position[1], &position[2]);
  sscanf(radiusStart + 7, "%lf,", &radius);

  // Catch invalid values
  if (color[0] == INFINITY ||
      color[1] == INFINITY ||
      color[2] == INFINITY ||
      position[0] == INFINITY ||
      position[1] == INFINITY ||
      position[2] == INFINITY ||
      radius == INFINITY) {
    return INVALID_PARSE_LINE;
  }
  else {

    // Populate sphere
    sphere->color = color;
    sphere->position = position;
    sphere->radius = radius;

    return 0;
  }
}

int parsePlane(plane_t *plane, char *line) {

  plane->kind = OBJECT_KIND_PLANE;

  // Variables to parse in to
  vector3_t color = vector3_create(INFINITY, INFINITY, INFINITY);
  vector3_t position = vector3_create(INFINITY, INFINITY, INFINITY);
  vector3_t normal = vector3_create(INFINITY, INFINITY, INFINITY);

  // Try to find width and height elements in file
  char *colorStart = strstr(line, "color:");
  char *positionStart = strstr(line, "position:");
  char *normalStart = strstr(line, "normal:");

  if (colorStart == NULL || positionStart == NULL || normalStart == NULL) {
    return INVALID_PARSE_LINE;
  }

  // Increment pointer beyond the initial scan string
  sscanf(colorStart + 6, " [%lf , %lf , %lf],",
    &color[0], &color[1], &color[2]);
  sscanf(positionStart + 9, " [%lf , %lf , %lf],",
    &position[0], &position[1], &position[2]);
  sscanf(normalStart + 7, " [%lf , %lf , %lf],",
    &normal[0], &normal[1], &normal[2]);

  // Catch invalid values
  if (color[0] == INFINITY ||
      color[1] == INFINITY ||
      color[2] == INFINITY ||
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
    plane->color = color;
    plane->position = position;
    plane->normal = normal;

    return 0;
  }
}

// < 0 == error, but > 0 the return value is the number of objects
int parseInput(camera_t *camera, object_t **scene, FILE *file) {

  int errorStatus = 0;
  int numObjects = 0;
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
    else if (strcmp(objectType, "sphere") == 0) {
      sphere_t *sphere = malloc(sizeof(sphere_t));
      errorStatus = parseSphere(sphere, line);

      // If no error, save object
      if (errorStatus == 0) {
        scene[numObjects++] = (object_t *) sphere;
      }
    }
    else if (strcmp(objectType, "plane") == 0) {
      plane_t *plane = malloc(sizeof(plane_t));
      errorStatus = parsePlane(plane, line);

      // If no error, save object
      if (errorStatus == 0) {
        scene[numObjects++] = (object_t *) plane;
      }
    }
  }

  // Ensure that a camera was found
  if (cameraFound != 0) {
    return NO_CAMERA_FOUND;
  }
  else return numObjects;
}