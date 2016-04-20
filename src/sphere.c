//
// Created by alexwang on 4/20/16.
//

#include "sphere.h"
#include <memory.h>
#include <stdio.h>
#include <math.h>
#include "object.h"
#include "algebra.h"


int sphere_intersection(const Object *object, const Ray *ray, Ray *reflect, Vector *inter, Vector *n) {

  Sphere *sphere = (Sphere*) object->priv;

  // ray.pos - sphere.pos
  Vector sub_result = sub(&ray->pos, &object->pos);
  // nearest point
  double t = -dot(&sub_result, &ray->front) / modulation2(&ray->front);

  Vector v = rmul(&ray->front, t);
  Vector nearest_point = add(&ray->pos, &v);

  // 检查交点是否在的正方向上
  Vector pb = sub(&nearest_point, &ray->pos);
  double inner_pb = dot(&pb, &ray->front);
  if (inner_pb < 0)
    return 0;

  // nearest distance
  Vector nearest_v = sub(&nearest_point, &object->pos);
  double nearest_dis2 = modulation2(&nearest_v);
  if (nearest_dis2 <= sphere->radius * sphere->radius) {
    double dis_nearest_to_inter = sqrt(sphere->radius * sphere->radius - nearest_dis2);
    Vector delta;

    Vector ray_direction = ray->front;
    normalize(&ray_direction);
    delta = rmul(&ray_direction, dis_nearest_to_inter);
    *inter = sub(&nearest_point, &delta);
    *n = sub(inter, &object->pos);

    // 计算反射光线方向
    *reflect = get_reflection_by_normal_and_ray(ray, &nearest_v, inter);
    return 1;
  }
  else {
    return 0;
  }
}

Vector sphere_normal_func(const struct TObject *object, const Vector *pos) {
  Vector direction = sub(pos, &object->pos);
  normalize(&direction);
  return direction;
}

Vector sphere_attenuation_func(const struct TObject *object,
                               const Ray *ray,
                               const Ray *reflect,
                               const Vector *pt,
                               const Vector *n) {
  const Sphere *sphere = (const Sphere*) object->priv;
  return sphere->color_attenuation;
}


//Vector sphere_intersect_color(const Object *object, const Ray *ray, Ray *reflect, Vector *inter, Vector *n) {
//  Sphere *ball = (Sphere*) object->priv;
//  double brightness = -dot(n, &ray->front) / modulation(n) / modulation(&ray->front);
//  return rmul(&ball->color, brightness);
//}
void sphere_init(Sphere *sp) {
  memset(&sp->object, 0, sizeof(Object));
  vector_init(&sp->object.pos);
  vector_init(&sp->object.front);
  vector_init(&sp->object.up);
  sp->object.priv = sp;

  sp->object.pos.x = 2.0;
  sp->object.type = OT_Ball;
  list_init(&sp->object.list);
  sp->object.intersection = sphere_intersection;
  sp->object.normal_func = sphere_normal_func;
  sp->object.attenuation_func = sphere_attenuation_func;
  sp->object.refraction_func = sphere_refraction_func;

  sp->radius = 0.3;
  sp->color.x = sp->color.y = sp->color.z = 1;
  sp->color_attenuation = v3(0.4, 0.4, 0.4);
}

int sphere_refraction_func(const struct TObject *object, const Ray *ray, const Ray *reflect, const Vector *pt,
                           const Vector *n, Ray *refract, Vector *attenuation) {

  // 入射角i

  const Sphere *sp = (const Sphere*)object->priv;
  double cosi = dot(n, &ray->front) / modulation(n) / modulation(&ray->front);
  double sini = sqrt(1 - cosi*cosi);

  double sinr;
  Vector vin = *n;
  if (cosi < 0) {
    // 空气到介质
    sinr = sini / sp->refractive;
  }

  else {
    // 介质到空气
    sinr = sini * sp->refractive;
    vin = rmul(n, -1);
    // 全反射
    if (sinr >= 1)
      return 0;
  }
  double r = asin(sinr);

  Vector left = vcross(vcross(*n, ray->front), *n);
  normalize(&left);
  Vector nn = *n;
  normalize(&nn);

  refract->front = vadd(vrmul(left, sin(r)), vrmul(nn, cos(r)));
  refract->pos = *pt;
  *attenuation = sp->color_attenuation;
  return 1;
}

