// Include header file
#include "raycast.h"


double rayObjectIntersect(object_t **outObject, object_t *skipObject,
                          vector3_t origin, vector3_t direction,
                          object_t **scene, int numObjects) {

  // Track closest object
  object_t *closestObject = NULL;
  double closestT = INFINITY;

  object_t *currObject = NULL;
  double currT;

  // Iterate through all objects to find nearest object
  for (int i = 0; i < numObjects; i++) {

    // Current object
    currObject = scene[i];

    if (skipObject != NULL && currObject == skipObject)
      continue;

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
    if (outObject != NULL)
      *outObject = closestObject;
    return closestT;
  }
}


vector3_t raycast(vector3_t origin, vector3_t direction,
                  object_t **scene, int numObjects,
                  object_t **lights, int numLights) {

  object_t *object;
  double t = rayObjectIntersect(&object, NULL,
                                origin, direction, scene, numObjects);

  // If we did not hit any objects, the pixel is in the void
  if (t == NO_INTERSECTION_FOUND) {
    return vector3_create(0, 0, 0); // Void color
  }
  else {

    // Calculate Shading

    vector3_t color = vector3_create(0, 0, 0); // TODO: Add ambient light
    vector3_t tempVector = vector3_create(0, 0, 0); // For calculations

    // Declare variables all only once
    light_t *light;
    vector3_t lIntersect = vector3_create(0, 0, 0);
    vector3_t lDirection = vector3_create(0, 0, 0);
    double lDistance;
    double shadowObjectT;

    double frad = 1;
    double fang = 1;
    vector3_t diff = vector3_create(0, 0, 0);
    vector3_t spec = vector3_create(0, 0, 0);
    vector3_t normal = vector3_create(0, 0, 0);
    vector3_t reflection = vector3_create(0, 0, 0);


    // For each light in the world
    for (int i = 0; i < numLights; i++) {

      light = (light_t *) lights[i];

      // Get intersection point
      vector3_scale(tempVector, direction, t);
      vector3_add(lIntersect, tempVector, origin);

      vector3_sub(lDirection, light->position, lIntersect); // Get l->o vector
      lDistance = vector3_mag(lDirection);

      vector3_normalize(lDirection); // Normalize light direction vector

      // Calculate reflection vector
      vector3_scale(tempVector, normal, vector3_dot(direction, normal)*2);
      vector3_sub(reflection, direction, tempVector);

      shadowObjectT = rayObjectIntersect(NULL, object, lIntersect,
                                         lDirection, scene, numObjects);

      // Only color the object if there isn't an object any closer
      if (shadowObjectT == NO_INTERSECTION_FOUND || shadowObjectT > lDistance) {

        frad = radialAttenuation(light, lDistance);
        fang = angularAttenuation(light, lDirection);
        
        if (object->kind == OBJECT_KIND_SPHERE) {
          sphere_t *sphere = (sphere_t *) object;
          vector3_sub(normal, lIntersect, sphere->position); // Calculate normal
          
          diffuseReflection(diff, sphere->diffuse_color, light->color,
                            normal, lDirection);
          specularReflection(spec, sphere->specular_color, light->color,
                               direction, reflection, 20);
        }
        else if (object->kind == OBJECT_KIND_PLANE) {
          plane_t *plane = (plane_t *) object;

          diffuseReflection(diff, plane->diffuse_color, light->color,
                            plane->normal, lDirection);
          specularReflection(spec, plane->specular_color, light->color,
                               direction, reflection, 20);
        }

        // Add to color channels
        color[0] += frad * fang * (diff[0] + spec[0]);
        color[1] += frad * fang * (diff[1] + spec[1]);
        color[2] += frad * fang * (diff[2] + spec[2]);
      }
    }

    // Clamp final color values
    color[0] = clampValue(color[0], 0.0, 1.0);
    color[1] = clampValue(color[1], 0.0, 1.0);
    color[2] = clampValue(color[2], 0.0, 1.0);

    // Clean up allocated memory
    free(tempVector);
    free(lIntersect);
    free(lDirection);
    free(diff);
    free(spec);
    free(normal);
    free(reflection);

    return color;
  }
}


// Actually creates and initializes the image, iterates over view plane
int renderImage(ppm_t *ppmImage, camera_t *camera,
                object_t **scene, int numObjects,
                object_t **lights, int numLights) {

  // Iterate over every pixel in the would be image
  double pixHeight = camera->height/ppmImage->height;
  double pixWidth = camera->width/ppmImage->width;

  // Declare the variables only once
  double yCoord;
  double xCoord;
  vector3_t direction = vector3_create(0, 0, 0);
  vector3_t color;

  for (int i = 0; i < ppmImage->height; i++) {
    yCoord = camera->height/2 - pixHeight * (i + 0.5);

    for (int j = 0; j < ppmImage->width; j++) {
      xCoord = -camera->width/2 + pixWidth * (j + 0.5);

      // Create direction vector
      direction[0] = xCoord;
      direction[1] = yCoord;
      direction[2] = -FOCAL_LENGTH;
      vector3_normalize(direction);

      // Get color from raycast
      color = raycast(camera->position, direction, scene, numObjects,
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

  // Handle open errors on output file
  if (!(outputFH = fopen(outputFName, "w"))) {
    fprintf(stderr, "Error: Unable to open '%s' for writing\n", outputFName);
    return 1;
  }

  // Write final raycasted product to the designated PPM file
  writePPM(ppmImage, outputFH, PPM_OUTPUT_VERSION);

  // Final program clean up
  fclose(inputFH);

  return 0;
}
