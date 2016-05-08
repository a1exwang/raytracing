//
// Created by alexwang on 4/19/16.
//

#include "world.h"
#include "object.h"
#include "list.h"
#include "sphere.h"
#include "plane.h"
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
  Sphere *sp = (Sphere*) malloc(sizeof(Sphere));
  sphere_init(sp);
  sp->object.pos = v3(5, 0, 2.4);
  sp->radius = 0.3;
  sp->object.name = "small sp";
  sp->reflect_attenuation = v3(0.1, 0.4, 0.4);
  sp->refract_attenuation = v3(0.3, 0.3, 0.3);
  sp->refractive = 1.5;
  world->first_object = &sp->object;

  sp = (Sphere*) malloc(sizeof(Sphere));
  sphere_init(sp);
  sp->object.name = "big sp";
  sp->object.pos = v3(2, 0, 2);
  sp->radius = 0.8;
  sp->reflect_attenuation = v3(0.4, 0.4, 0.4);
  sp->refract_attenuation = v3(0.3, 0.3, 0.3);
  sp->refractive = 1.3;
  list_insert_before(&world->first_object->list, &sp->object.list);

//  Plane *plane = (Plane*) malloc(sizeof(Plane));
//  plane_init(plane);
//  plane->object.name = "floor";
//  plane->object.pos = v3(0, 0, 4);
//  plane->object.front = v3(0, 0, -1);
//  plane->object.up = v3(1, 0, 0);
//  plane->reflective_attenuation = v3(0.3, 0.3, 0.3);
//  plane->object.reflective_attenuation_func = NULL;
//  list_insert_before(&world->first_object->list, &plane->object.list);
 // world->first_object = &plane->object;

  world->ambient_light = v3(0.01, 0.01, 0.01);

  Light *light = (Light*) malloc(sizeof(Light));
  spot_light_init(light, 0.5, 0.5, 0.5);
  light->pos = v3(0, 0.50, -1);
  world->first_light = light;

//  light = (Light*) malloc(sizeof(Light));
//  spot_light_init(light, 0.5, 0.5, 0.5);
//  light->pos = v3(0, 1, 3);
//  list_insert(&world->first_light->list, &light->list);
}

void world_bind_camera(World *world, Camera *camera) {
  world->camera = camera;
  camera->world = world;
}

Object *world_closest_object(const World *world, const Ray *ray, Vector *intersection, Ray *reflect, Vector *n) {

  Object *nearest = NULL;
  double nearest_distance = world_max_distance(world);

  LIST_FOREACH(&world->first_object->list, Object, list, object)
      int success = 0;
      if (object->intersection(object, ray, reflect, intersection, n)) {
        double dis = ray_distance(ray, intersection, &success);
        if (dis >= 0 && dis < nearest_distance) {
          nearest_distance = dis;
          nearest = object;
        }
      }
  LIST_FOREACH_END()
  return nearest;
}
