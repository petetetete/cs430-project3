// Include header file
#include "raycast.h"


vector3_t calculateShading(vector3_t direction, object_t *object, double t,
                           object_t **scene, int numObjects,
                           object_t **lights, int numLights) {

  vector3_t color = vector3_create(0, 0, 0); // Replace with ambient light

  // For each light in the world
  for (int i = 0; i < numLights; i++) {

    light_t *light = (light_t *) lights[i];

    vector3_t intersection = vector3_scale(direction, t);
    vector3_t lDirection = vector3_sub(light->position, intersection);
    double lDistance = vector3_mag(lDirection);

   /* // Save closest shadow casting object
    //object_t *closestObject = NULL;
    double closestT = INFINITY;

    for (int j = 0; j < numObjects; j++) {
      object_t *currentObject = scene[j];
      double currentT;

      if (currentObject == object) continue; // Skip checking the current object

      switch (currentObject->kind) {
        case OBJECT_KIND_SPHERE:
          currentT = sphereIntersect(lDirection, (sphere_t *) currentObject);
          break;
        case OBJECT_KIND_PLANE:
          currentT = planeIntersect(lDirection, (plane_t *) currentObject);
          break;
      }

      // If the current t was closer than all before, save it
      if (currentT != NO_INTERSECTION_FOUND && currentT < closestT) {
        closestT = currentT;
        //closestObject = currentObject;
      }
    }*/

    // closestT > lDistance && closestObject == NULL
    if (1) {

      // Calculate color
      double frad = radialAttenuation(light, lDistance);
      double fang = angularAttenuation(light, lDirection);
      
      if (object->kind == OBJECT_KIND_SPHERE) {
        sphere_t *sphere = (sphere_t *) object;
        color[0] += frad * fang * (sphere->diffuse_color[0] + sphere->specular_color[0]);
        color[1] += frad * fang * (sphere->diffuse_color[1] + sphere->specular_color[1]);
        color[2] += frad * fang * (sphere->diffuse_color[2] + sphere->specular_color[2]);
      }
      else if (object->kind == OBJECT_KIND_PLANE) {
        plane_t *plane = (plane_t *) object;
        color[0] += frad * fang * (plane->diffuse_color[0] + plane->specular_color[0]);
        color[1] += frad * fang * (plane->diffuse_color[1] + plane->specular_color[1]);
        color[2] += frad * fang * (plane->diffuse_color[2] + plane->specular_color[2]);
      }
    }
  }

  color[0] = clampValue(color[0], 0.0, 1.0);
  color[1] = clampValue(color[1], 0.0, 1.0);
  color[2] = clampValue(color[2], 0.0, 1.0);

  return color;
}


double rayObjectIntersect(object_t **outObject,
                          vector3_t origin, vector3_t direction,
                          object_t **scene, int numObjects) {

  // Track closet object
  object_t *closestObject = NULL;
  double closestT = INFINITY;

  object_t *currObject = NULL;
  double currT;

  // Iterate through all objects to find nearest object
  for (int i = 0; i < numObjects; i++) {

    // Current object
    currObject = scene[i];

    // Determine which intersection type to check for
    switch (currObject->kind) {
      case OBJECT_KIND_SPHERE:
        currT = sphereIntersect(origin, direction, (sphere_t *) currObject);
        break;
      case OBJECT_KIND_PLANE:
        currT = planeIntersect(origin, direction, (plane_t *) currObject);
        break;
    }

    // If the current t was closer than all before, save the color
    if (currT != NO_INTERSECTION_FOUND && currT < closestT) {
      closestT = currT;
      closestObject = currObject;
    }
  }

  if (closestObject == NULL) {
    outObject = NULL;
    return NO_INTERSECTION_FOUND;
  }
  else {
    *outObject = closestObject;
    return closestT;
  }
}


vector3_t raycast(vector3_t origin, vector3_t direction,
                  object_t **scene, int numObjects,
                  object_t **lights, int numLights) {

  object_t *object;
  double t = rayObjectIntersect(&object, origin, direction, scene, numObjects);

  // If we did not hit any objects, the pixel is in the void
  if (t == NO_INTERSECTION_FOUND) {
    return vector3_create(0, 0, 0); // Void color
  }
  else {

    /* TODO: remove
    switch (closestObject->kind) {
      case OBJECT_KIND_SPHERE:
        return ((sphere_t *) closestObject)->diffuse_color;
      case OBJECT_KIND_PLANE:
        return ((plane_t *) closestObject)->diffuse_color;
    }*/

    vector3_t color = calculateShading(direction, object, t,
                                       scene, numObjects,
                                       lights, numLights);
    return color;
  }
}


// Actually creates and initializes the image
int renderImage(ppm_t *ppmImage, camera_t *camera,
                object_t **scene, int numObjects,
                object_t **lights, int numLights) {

  // Iterate over every pixel in the would be image
  double pixHeight = camera->height/ppmImage->height;
  double pixWidth = camera->width/ppmImage->width;

  for (int i = 0; i < ppmImage->height; i++) {
    double yCoord = camera->height/2 - pixHeight * (i + 0.5);

    for (int j = 0; j < ppmImage->width; j++) {
      double xCoord = -camera->width/2 + pixWidth * (j + 0.5);

      // Create direction vector
      vector3_t direction = vector3_createUnit(xCoord, yCoord, -FOCAL_LENGTH);

      // Get color from raycast
      vector3_t color = raycast(camera->position, direction,
                                scene, numObjects,
                                lights, numLights);

      // Populate pixel with color data
      ppmImage->pixels[i*ppmImage->width + j].r = (int) (color[0] * 255);
      ppmImage->pixels[i*ppmImage->width + j].g = (int) (color[1] * 255);
      ppmImage->pixels[i*ppmImage->width + j].b = (int) (color[2] * 255);
    }
  }

  return 0; // No errors!
}


int main(int argc, char *argv[]) {

  // Check for the appropriate number of parameters
  if (argc != 5) {
    fprintf(stderr, USAGE_MESSAGE);
    return 1;
  }

  // Save command line parameters
  int viewWidth = atoi(argv[1]);
  int viewHeight = atoi(argv[2]);
  char *inputFName = argv[3];
  char *outputFName = argv[4];

  if (viewWidth <= 0 || viewHeight <= 0) {
    fprintf(stderr, "Error: Invalid width or height, must be > 0\n");
    return 1;
  }

  // Initialize variables to be used in program
  FILE *inputFH;
  FILE *outputFH;
  camera_t *camera = malloc(sizeof(camera_t));
  object_t **scene = malloc(sizeof(object_t) * MAX_SCENE_OBJECTS);
  object_t **lights = malloc(sizeof(object_t) * MAX_SCENE_LIGHTS);
  int *numObjects;

  // Create final ppmImage
  ppm_t *ppmImage = malloc(sizeof(ppm_t));
  ppmImage->width = viewWidth;
  ppmImage->height = viewHeight;
  ppmImage->maxColorValue = 255;
  ppmImage->pixels = malloc(sizeof(pixel_t)*ppmImage->width*ppmImage->height);

  // Handle input file errors
  if (!(inputFH = fopen(inputFName, "r"))) {
    fprintf(stderr, "Error: Input file '%s' could not be found\n", inputFName);
    return 1;
  }

  // Parse input csv into scene object
  numObjects = parseInput(camera, scene, lights, inputFH);

  // Handle errors found in parseInput
  if (numObjects == NULL) {
    fprintf(stderr, "Error: Malformed input CSV\n");
    return 1;
  }

  // Create actual PPM image from scene
  renderImage(ppmImage, camera, scene, numObjects[0], lights, numObjects[1]);

  // Clean up allocated memory
  free(camera);
  free(scene);
  free(lights);

  // Handle open errors on output file
  if (!(outputFH = fopen(outputFName, "w"))) {
    fprintf(stderr, "Error: Unable to open '%s' for writing\n", outputFName);
    return 1;
  }

  // Write final raycasted product to the designated PPM file
  writePPM(ppmImage, outputFH, PPM_OUTPUT_VERSION);

  // Final program clean up
  free(ppmImage->pixels);
  free(ppmImage);
  fclose(inputFH);

  return 0;
}
