//
// Created by alexwang on 4/19/16.
//

#ifndef RAYTRACING_WORLD_H
#define RAYTRACING_WORLD_H

#include "algebra.h"
#include "camera.h"

typedef struct TWorld {
  struct TCamera *camera;
  Object *first_object;
  Light *light;
} World;

void world_init(World *world);
void world_zygote(World *world);
void world_bind_camera(World *world, struct TCamera *camera);

#endif //RAYTRACING_WORLD_H
