//
// Created by alexwang on 4/19/16.
//

#include "world.h"
#include "object.h"
#include "list.h"
#include "sphere.h"
#include <math.h>

void world_init(World *world) {
  world->first_object = NULL;
  world->first_light = NULL;
  world->zmin = world->xmin = world->ymin = -10;
  world->zmax = world->xmax = world->ymax = 10;
  world->max_distance = sqrt(world->xmin * world->xmin + world->ymin * world->ymin + world->zmin * world->zmax);
}

double world_max_distance(const World *world) {
  return world->max_distance;
}

void world_zygote(World *world) {
  Sphere *ball = (Sphere*) malloc(sizeof(Sphere));
  sphere_init(ball);
  ball->object.pos.x = 4.5;
  ball->object.pos.y = 0;
  ball->object.pos.z = 0.3;
  ball->radius = 1.2;
  ball->object.name = "big ball";
  world->first_object = &ball->object;

  ball = (Sphere*) malloc(sizeof(Sphere));
  sphere_init(ball);
  ball->object.name = "small ball";
  ball->object.pos.x = 2;
  ball->object.pos.y = 0;
  ball->object.pos.z = 0;
  ball->radius = 0.5;
  ball->color = v3(1, 0, 0);
  list_insert_before(&world->first_object->list, &ball->object.list);

  world->ambient_light.x = world->ambient_light.y = world->ambient_light.z = 0.01;

  Light *light = (Light*) malloc(sizeof(Light));
  spot_light_init(light, 1, 1, 1);
  light->pos.x = 4;
  light->pos.y = 0;
  light->pos.z = 20;
  world->first_light = light;

  light = (Light*) malloc(sizeof(Light));
  spot_light_init(light, 0, 1, 1);
  light->pos.x = 0;
  light->pos.y = 1;
  light->pos.z = -5.2;

  list_insert(&world->first_light->list, &light->list);
}

void world_bind_camera(World *world, Camera *camera) {
  world->camera = camera;
  camera->world = world;
}

Object *world_closest_object(const World *world, const Ray *ray, Vector *intersection, Ray *reflect) {

  Object *nearest = NULL;
  double nearest_distance = world_max_distance(world);

  LIST_FOREACH(&world->first_object->list, Object, list, object)
      Vector n;
      int success = 0;
      if (object->intersection(object, ray, reflect, intersection, &n)) {
        double dis = ray_distance(ray, intersection, &success);
        if (dis >= 0 && dis < nearest_distance) {
          nearest_distance = dis;
          nearest = object;
        }
      }
  LIST_FOREACH_END()
  return nearest;
}

// 获得所有光源, 在物体某个位置的光照颜色
Vector get_light_color(World *world, const Vector *pos, const Object *object) {
  Vector total = { 0, 0, 0 };
  int i = 0;
  LIST_FOREACH(&world->first_light->list, Light, list, light)
    Vector color = light->diffuse_func(light, world, pos, object);
    //printf("light %d: %f, %f, %f\t", i++, color.x, color.y, color.z);
    total = add(&total, &color);
  LIST_FOREACH_END()
  //printf("\n");
  return total;
}

// 递归跟踪某一条光线
Vector ray_trace(World *world, Ray *ray, int trace_times) {
  if (trace_times <= 0)
    return color_black();

  Vector intersection;
  Ray reflect;
  // 该光线直接射中物体
  Object *object = world_closest_object(world, ray, &intersection, &reflect);
  if (!object)
    return color_black();

  Vector light_color = get_light_color(world, &intersection, object);
  Vector add_ambient = add(&light_color, &world->ambient_light);

  return add_ambient;
  Vector color2 = ray_trace(world, &reflect, trace_times - 1);
  // color2需要衰减一下
  color2 = rmul(&color2, 0.5);
  /*printf("light: %f, %f, %f, recur: %f, %f, %f\n",
         light_color.x, light_color.y, light_color.z,
         color2.x, color2.y, color2.z);*/


  return add(&color2, &light_color);
}
