#include "object.h"

Ray get_reflection_by_normal_and_ray(const Ray *ray, const Vector *normal, const Vector *intersection) {
  Ray reflect;
  reflect.pos = *intersection;
  Vector n = *normal;

  normalize(&n);
  double costheta = dot(&ray->front, &n) / modulation(&ray->front);
  Vector ray_normal_v = rmul(&n, modulation(&ray->front) * costheta);
  Vector two_n = rmul(&ray_normal_v, 2);
  reflect.front = add(&ray->front, &two_n);
  return reflect;
}