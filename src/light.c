//
// Created by alexwang on 4/18/16.
//

#include "light.h"
void spot_light_init(Light *light, double r, double g, double b) {
  list_init(&light->list);
  light->type = Light_Spot;
  light->color.x = r;
  light->color.y = g;
  light->color.z = b;
  light->diffuse_func = spot_light_diffuss_func;
  light->priv = NULL;
}

Vector spot_light_diffuss_func(const Light *light, const World *world, const Vector *pos, const Object *object) {
  Vector direction = sub(&light->pos, pos);
  normalize(&direction);

  Ray ray = { .front = direction, .pos = *pos };
  Vector inter; Ray refl;
  Object *closest = world_closest_object(world, &ray, &inter, &refl);
  if (closest && ((closest != object) || !same_vector(&inter, pos))) {
    return color_black();
  }
  Vector normal = object->normal_func(object, pos);
  double cos_theta = -dot(&normal, &direction) / modulation(&normal) / modulation(&direction);
  if (cos_theta >= 0) {
    return rmul(&light->color, cos_theta);
  }
  else {
    return color_black();
  }
}