//
// Created by alexwang on 4/19/16.
//

#include "world.h"
#include "object.h"
#include "list.h"

void world_init(World *world) {
  world->first_object = NULL;
  world->light = NULL;
}

void world_zygote(World *world) {
  Ball *ball = (Ball*) malloc(sizeof(Ball));
  ball_init(ball);
  ball->object.pos.x = 2;
  ball->object.pos.y = -0.5;
  ball->object.pos.z = 0.2;
  ball->radius = 0.1;
  world->first_object = &ball->object;

  ball = (Ball*) malloc(sizeof(Ball));
  ball_init(ball);
  ball->object.pos.x = 2;
  ball->object.pos.y = 0.5;
  ball->object.pos.z = -0.2;
  ball->radius = 1;
  ball->color = v3(1.0, 1, 0);
  list_insert_before(&world->first_object->list, &ball->object.list);

  SurfaceLight *light = (SurfaceLight*) malloc(sizeof(SurfaceLight));
  surface_light_init(light, 1, 0, 1);
  world->light = &light->light;
}

void world_bind_camera(World *world, Camera *camera) {
  world->camera = camera;
  camera->world = world;
}