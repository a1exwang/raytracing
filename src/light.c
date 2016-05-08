//
// Created by alexwang on 4/18/16.
//

#include <math.h>
#include "light.h"
void spot_light_init(Light *light, double r, double g, double b) {
  list_init(&light->list);
  light->type = Light_Spot;
  light->color.x = r;
  light->color.y = g;
  light->color.z = b;
  light->diffuse_func = spot_light_diffuse_func;
  light->reflective_func = spot_light_reflective_func;
  light->priv = NULL;
}

Vector spot_light_diffuse_func(const Light *light, const World *world, const Vector *pos, const Object *object) {
  Vector direction = sub(&light->pos, pos);
  normalize(&direction);

  Ray ray = { .front = direction, .pos = *pos };
  Vector inter, n; Ray refl;
  Object *closest = world_closest_object(world, &ray, &inter, &refl, &n);
  if (closest  && ((closest != object) || !same_vector(&inter, pos))) {
    return world->ambient_light;
  }
  Vector normal = object->normal_func(object, pos);
  double cos_theta = dot(&normal, &direction) / modulation(&normal) / modulation(&direction);
  if (cos_theta >= 0) {
    return rmul(&light->color, 0.5);
  }
  else {
    return world->ambient_light;
  }
}

Vector spot_light_reflective_func(const Light *light,
                                  const struct TWorld *world,
                                  const Vector *pos,
                                  const Object *object,
                                  const Ray * __ray) {
  Vector direction = sub(&light->pos, pos);
  normalize(&direction);

  Ray ray = { .front = direction, .pos = *pos };
  Vector inter, n; Ray refl;
  Object *closest = world_closest_object(world, &ray, &inter, &refl, &n);
  if (closest  && ((closest != object) || !same_vector(&inter, pos))) {
    return world->ambient_light;
  }
  Vector normal = object->normal_func(object, pos);
  double cos_theta = dot(&normal, &direction) / modulation(&normal) / modulation(&direction);
  // 光源在反射光线附近做一个高斯模糊
  double theta = acos(cos_theta);
  double v = 0.1;
  double k = exp(-theta * theta / v / v);
  return vrmul(light->color, k);
}