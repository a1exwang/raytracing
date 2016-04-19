//
// Created by alexwang on 4/18/16.
//

#include "light.h"
void surface_light_init(SurfaceLight *light, double r, double g, double b) {
  light->color.x = r;
  light->color.y = g;
  light->color.z = b;
}
