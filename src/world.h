//
// Created by alexwang on 4/19/16.
//

#ifndef RAYTRACING_WORLD_H
#define RAYTRACING_WORLD_H

#include "algebra.h"
#include "camera.h"
#include "light.h"

typedef struct TWorld {
  struct TCamera *camera;
  Object *first_object;
  struct TLight *first_light;
  double xmin, xmax, ymin, ymax, zmin, zmax;
  double max_distance;
  Vector ambient_light;
} World;

void world_init(World *world);
void world_zygote(World *world);
void world_bind_camera(World *world, struct TCamera *camera);
double world_max_distance(const World *world);
Object *world_closest_object(const World *world, const Ray *ray, Vector *intersection, Ray *reflect);

Vector ray_trace(World *world, Ray *ray, int trace_times);

#endif //RAYTRACING_WORLD_H
