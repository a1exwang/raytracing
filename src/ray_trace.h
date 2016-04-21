//
// Created by alexwang on 4/21/16.
//

#ifndef RAYTRACING_RAY_TRACE_H
#define RAYTRACING_RAY_TRACE_H

#include "algebra.h"
#include "world.h"

enum {
  RT_None,
  RT_Ambient,
  RT_Diffuse,
  RT_Reflection,
  RT_Refraction,
  RT_Root
};

typedef struct TRayTrace {
  /* in parameters */
  const World *world;
  int trace_times;

  Ray ray;
  Vector att;

  struct TRayTrace *parent;
  int type;

  Vector color;

  int x, y;
  //Vector **buffer;
} RayTrace;

void ray_trace(World *world, Ray *ray, int trace_times, int x, int y, Vector **buffer);


#endif //RAYTRACING_RAY_TRACE_H
