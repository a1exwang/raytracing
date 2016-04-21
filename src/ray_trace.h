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
  RT_Refraction
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

Vector ray_trace(World *world, Ray *ray, int trace_times);


#endif //RAYTRACING_RAY_TRACE_H
