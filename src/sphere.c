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
    reflect->pos = *inter;

    // 计算反射光线方向
    normalize(&nearest_v);
    double costheta = dot(&ray->front, &nearest_v) / modulation(&ray->front);
    Vector ray_normal_v = rmul(&nearest_v, modulation(&ray->front) * costheta);
    Vector two_n = rmul(&ray_normal_v, 2);
    reflect->front = add(&ray->front, &two_n);
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

//Vector sphere_intersect_color(const Object *object, const Ray *ray, Ray *reflect, Vector *inter, Vector *n) {
//  Sphere *ball = (Sphere*) object->priv;
//  double brightness = -dot(n, &ray->front) / modulation(n) / modulation(&ray->front);
//  return rmul(&ball->color, brightness);
//}
void sphere_init(Sphere *ball) {
  memset(&ball->object, 0, sizeof(Object));
  vector_init(&ball->object.pos);
  vector_init(&ball->object.front);
  vector_init(&ball->object.up);
  ball->object.priv = ball;

  ball->object.pos.x = 2.0;
  ball->object.type = OT_Ball;
  list_init(&ball->object.list);
  ball->object.intersection = sphere_intersection;
  ball->object.normal_func = sphere_normal_func;
  //ball->object.color = //sphere_intersect_color;

  ball->radius = 0.3;
  ball->color.x = ball->color.y = ball->color.z = 1;
}
