//
// Created by alexwang on 4/20/16.
//

#ifndef RAYTRACING_PLANE_H
#define RAYTRACING_PLANE_H

#include "object.h"

// object.front 是法向量
// 对于有限矩形面来说,
// object.up 是width边方向
typedef struct TPlane {
  Object object;
  int unlimited;
  double width, height;
  Vector reflective_attenuation;
} Plane;

void plane_init(Plane *plane);
Vector plane_normal_func(const Object *plane, const Vector *pos);
int plane_intersection(const Object *plane, const Ray *ray, Ray *reflect, Vector *inter, Vector *n);


#endif //RAYTRACING_PLANE_H
