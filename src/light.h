//
// Created by alexwang on 4/18/16.
//

#ifndef RAYTRACING_LIGHT_H
#define RAYTRACING_LIGHT_H

#include "algebra.h"

typedef struct {
  Vector pos;
  void *priv;
} Light;

typedef struct {
  Light light;
  Vector left_top;
  Vector right_top;
  Vector color;
} SurfaceLight;
void surface_light_init(SurfaceLight *light, double r, double g, double b);

#endif //RAYTRACING_LIGHT_H
