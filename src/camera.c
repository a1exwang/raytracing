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

void camera_shot(Camera *camera, Bitmap *bitmap) {
  srand((unsigned) time(NULL));

  Ball *ball = (Ball*) camera->object;

  // 对相机上的每一个点反向追踪
  for (int x = 0; x < camera->width; ++x) {
    for (int y = 0; y < camera->height; ++y) {
      vector xnorm, ynorm;
      vector relpos, xrel, yrel;

      ynorm = camera->up;
      xnorm = cross(&camera->up, &camera->front);

      normalize(&ynorm);
      normalize(&xnorm);

      xrel = rmul(&xnorm, ((double)x / camera->width - 0.5) * camera->viewport_width);
      yrel = rmul(&ynorm, ((double)y / camera->height -0.5) * camera->viewport_height);

      relpos = add(&xrel, &yrel);
      vector pos = add(&camera->pos, &relpos);

      Ray ray = {
              .front = camera->front,
              .pos = pos
      };
      vector color;

      vector inter, v;
      const vector *ray_vec = &ray.front;

      int has_intersection = ball_intersect(ball, &ray, &inter, &v);

      if (has_intersection) {
        double brightness = -dot(&v, ray_vec) / modulation(&v) / modulation(ray_vec);
        if (brightness < 0)
          brightness = 0;

        color.x = fabs(brightness);
        color.y = fabs(brightness);
        color.z = fabs(brightness);
      }
      else {
        color.x = 0;
        color.y = 0;
        color.z = 0;
      }

      bitmap->buffer[x][y] = color;
    }
  }
}

void camera_init(Camera *c, int width, int height)  {
  c->width = width;
  c->height = height;

  Ball *ball = (Ball*) malloc(sizeof(Ball));
  ball_init(ball);
  c->object = &ball->object;
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