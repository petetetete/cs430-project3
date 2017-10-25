// Include header file
#include "raycast.h"

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
  vector3_t subVector = vector3_create(0, 0, 0);;
  vector3_sub(subVector, plane->position, direction);
  double t = vector3_dot(subVector, plane->normal) / product;

  // Only return t when it is a positive scalar
  if (t > 0) {
    return t;
  }
  else return NO_INTERSECTION_FOUND;
}

vector3_t raycast(object_t **scene, vector3_t direction, int numObjects) {

  // Track closet object
  vector3_t closestColor = vector3_create(0, 0, 0); // Default to black
  double closestT = INFINITY;

  // Iterate through all objects to find nearest color
  for (int i = 0; i < numObjects; i++) {

    // Current object and t value
    object_t *object = scene[i];
    vector3_t color;
    double t;

    // Determine which intersection type to check for
    if (object->kind == OBJECT_KIND_SPHERE) {
      sphere_t *sphere = (sphere_t *) object; // Convert to sphere
      color = sphere->color;
      t = sphereIntersection(direction, sphere);
    }
    else if (object->kind == OBJECT_KIND_PLANE) {
      plane_t *plane = (plane_t *) object; // Convert to plane
      color = plane->color;
      t = planeIntersection(direction, plane);
    }

    // If the current t was closer than all before, save the color
    if (t != NO_INTERSECTION_FOUND && t < closestT) {
      closestT = t;
      closestColor = color;
    }
  }

  return closestColor;
}

// Actually creates and initializes the image
int renderImage(ppm_t *ppmImage, camera_t *camera,
                object_t **scene, int numObjects) {

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
      vector3_t color = raycast(scene, direction, numObjects);

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
  int numObjects;

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
  numObjects = parseInput(camera, scene, inputFH);

  // Handle errors found in parseInput
  if (numObjects < 0) {
    fprintf(stderr, "Error: Malformed input CSV\n", outputFName);
    return 1;
  }

  // Create actual PPM image from scene
  renderImage(ppmImage, camera, scene, numObjects);

  // Handle open errors on output file
  if (!(outputFH = fopen(outputFName, "w"))) {
    fprintf(stderr, "Error: Unable to open '%s' for writing\n", outputFName);
    return 1;
  }

  // Write final raycasted product to the designated PPM file
  writePPM(ppmImage, outputFH, PPM_OUTPUT_VERSION);

  // Clean up program
  fclose(inputFH);

  return 0;
}
