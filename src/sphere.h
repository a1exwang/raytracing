//
// Created by alexwang on 4/20/16.
//

#ifndef RAYTRACING_SPHERE_H
#define RAYTRACING_SPHERE_H

#include "object.h"

typedef struct TSphere {
  Object object;
  double radius;
  Vector color;
  Vector reflect_attenuation;
  Vector refract_attenuation;
  double refractive;
} Sphere;

void sphere_init(Sphere *sp);
Vector sphere_normal_func(const struct TObject *object, const Vector *pos);
int sphere_intersection(const Object *sphere, const Ray *ray, Ray *reflect, Vector *inter, Vector *n);

int sphere_refraction_func(const struct TObject *object, const Ray *ray, const Ray *reflect,
                               const Vector *pt, const Vector *n, Ray *refract, Vector *attenuation);

#endif //RAYTRACING_SPHERE_H
