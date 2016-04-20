//
// Created by alexwang on 4/18/16.
//

#include <memory.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "bitmap.h"
#include "camera.h"
#include "object.h"
#include "algebra.h"
#include "light.h"
#include "list.h"

Ray camera_lens_func_plane(const Camera *camera, int x, int y) {
  CameraLensPlaneData *plane = (CameraLensPlaneData*)camera->lens_data;
  Vector delta = rmul(&camera->front, -plane->image_distance);
  Vector eye = add(&camera->pos, &delta);

  Vector xnorm, ynorm;
  Vector relpos, xrel, yrel;

  ynorm = camera->up;
  xnorm = cross(&camera->up, &camera->front);

  normalize(&ynorm);
  normalize(&xnorm);

  xrel = rmul(&xnorm, ((double)x / camera->width - 0.5) * camera->viewport_width);
  yrel = rmul(&ynorm, ((double)y / camera->height -0.5) * camera->viewport_height);

  relpos = add(&xrel, &yrel);
  Vector pos = add(&camera->pos, &relpos);

  Vector front = sub(&pos, &eye);
  Ray ray = {
          .front = front,
          .pos = pos
  };
  return ray;
}

void camera_render(Camera *camera, Bitmap *bitmap) {
  srand((unsigned) time(NULL));

  // 对相机上的每一个点反向追踪
  for (int x = 0; x < camera->width; ++x) {
    for (int y = 0; y < camera->height; ++y) {
      Ray ray = camera->lens_func(camera, x, y);
      Vector color = ray_trace(camera->world, &ray, 10);
      //printf("color: %f, %f, %f\n", color.x, color.y, color.z);
      bitmap->buffer[x][y] = color;
    }
  }
}

void camera_init(Camera *c, int width, int height)  {
  c->width = width;
  c->height = height;

  CameraLensPlaneData *data = (CameraLensPlaneData*) malloc(sizeof(CameraLensPlaneData));
  data->image_distance = 3;

  c->lens_func = camera_lens_func_plane;
  c->lens_data = data;
}

void camera_goto(Camera *c, double x, double y, double z) {
  c->pos.x = x;
  c->pos.y = y;
  c->pos.z = z;
}

void camera_ortho(Camera *c, double xmin, double xmax, double ymin, double ymax, double zmin, double zmax) {
  c->xmin = xmin;
  c->xmax = xmax;
  c->ymin = ymin;
  c->ymax = ymax;
  c->zmin = zmin;
  c->zmax = zmax;
}

void camera_viewport(Camera *c,
                     double front_x, double front_y, double front_z,
                     double up_x, double up_y, double up_z,
                     double view_width, double view_height) {
  c->front.x = front_x;
  c->front.y = front_y;
  c->front.z = front_z;

  c->up.x = up_x;
  c->up.y = up_y;
  c->up.z = up_z;

  c->viewport_width = view_width;
  c->viewport_height = view_height;
}


