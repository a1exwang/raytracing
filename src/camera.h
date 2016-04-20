//
// Created by alexwang on 4/18/16.
//

#ifndef RAYTRACING_CAMERA_H
#define RAYTRACING_CAMERA_H

#include <malloc.h>
#include "algebra.h"
#include "object.h"
#include "bitmap.h"
#include "light.h"
#include "world.h"

struct TCamera;

typedef Ray (*CameraLensFunc)(const struct TCamera* camera, int x, int y);

typedef struct TCamera {
  Vector pos;
  Vector front;
  Vector up;

  double viewport_width, viewport_height;
  int width, height;

  double xmin, xmax;
  double ymin, ymax;
  double zmin, zmax;

  struct TWorld *world;
  CameraLensFunc lens_func;
  void *lens_data;
} Camera;

typedef struct TCameraLensPlaneData {
  double image_distance;
} CameraLensPlaneData;

typedef struct TCameraLensBallData {
  double horizontal_view_degree;
  double vertical_view_degree;
  Vector center_position;
  double radius;
} CameraLensBallData;

void camera_init(Camera *c, int width, int height);
void camera_goto(Camera *c, double x, double y, double z);
void camera_ortho(Camera *c, double xmin, double xmax, double ymin, double ymax, double zmin, double zmax);
void camera_viewport(Camera *c, double front_x, double front_y, double front_z, double up_x, double up_y, double up_z,
                     double view_width, double view_height);

void camera_render(Camera *camera, Bitmap *bitmap);



#endif //RAYTRACING_CAMERA_H
