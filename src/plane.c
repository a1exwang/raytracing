//
// Created by alexwang on 4/20/16.
//

#include "plane.h"
#include "algebra.h"
#include <memory.h>
Vector plane_reflective_attenuation_func(const struct TObject *object,
                                         const Ray *ray,
                                         const Ray *reflect,
                                         const Vector *pt,
                                         const Vector *n) {
  const Plane *plane = (const Plane*) object->priv;
  return plane->reflective_attenuation;
}


void plane_init(Plane *plane) {
  memset(&plane->object, 0, sizeof(Object));
  vector_init(&plane->object.pos);
  plane->object.front = v3(0, 0, 1); // 法向量
  plane->object.up = v3(1, 0, 0); // width
  plane->object.priv = plane;

  plane->object.pos.x = 2.0;
  plane->object.type = OT_Plane;
  plane->unlimited = 1;
  list_init(&plane->object.list);
  plane->object.intersection = plane_intersection;
  plane->object.normal_func = plane_normal_func;
  plane->object.reflective_attenuation_func = plane_reflective_attenuation_func;
  plane->object.refraction_func = NULL;

  plane->reflective_attenuation = v3(0.4, 0.4, 0.4);
}

int plane_intersection(const Object *object, const Ray *ray, Ray *reflect, Vector *inter, Vector *n) {
  const Plane *plane = (const Plane*) object->priv;

  // ri: 面上的点, ni: 面的法向量
  // pi: ray的起点, ki: ray的方向
  const Vector *ri = &object->pos;
  const Vector *ni = &object->front;
  const Vector *pi = &ray->pos;
  const Vector *ki = &ray->front;

  Vector ri_minus_pi = sub(ri, pi);
  double t = dot(&ri_minus_pi, ni) / dot(ki, ni);

  Vector mul_result = rmul(ki, t);
  *inter = add(&mul_result, pi);
  *reflect = get_reflection_by_normal_and_ray(ray, ni, inter);
  *n = *ni;
  //if (plane->unlimited) {
  Vector v = sub(inter, &ray->pos);
  if (dot(&ray->front, &v) < 0) {
    return 0;
  }

  return 1;
  //}
}

Vector plane_normal_func(const Object *plane, const Vector *pos) {
  return plane->front;
}