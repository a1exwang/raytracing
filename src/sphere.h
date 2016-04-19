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
} Sphere;


void sphere_init(Sphere *ball);
Vector sphere_normal_func(const struct TObject *object, const Vector *pos);
int sphere_intersection(const Object *sphere, const Ray *ray, Ray *reflect, Vector *inter, Vector *n);
Vector sphere_intersect_color(const Object *sphere, const Ray *ray, Ray *reflect, Vector *inter, Vector *n);


#endif //RAYTRACING_SPHERE_H
