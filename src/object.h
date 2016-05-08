//
// Created by alexwang on 4/18/16.
//

#ifndef RAYTRACING_OBJECT_H
#define RAYTRACING_OBJECT_H

#include "algebra.h"
#include "list.h"

enum {
  OT_Ball,
  OT_Plane
};

struct TObject;

typedef Vector (*NormalFunc) (const struct TObject *object, const Vector *pos);
typedef int (*IntersectFunc)(const struct TObject *object, const Ray *ray, Ray *reflect, Vector *pt, Vector *n);
typedef int (*RefractionFunc) (const struct TObject *object, const Ray *ray, const Ray *reflect,
                               const Vector *pt, const Vector *n, Ray *refract, Vector *attenuation);
typedef Vector (*AttenuationFunc)(const struct TObject *object, const Ray *ray, const Ray *reflect, const Vector *pt, const Vector *n);

typedef struct TObject {
  ListHead list;

  char *name;
  int type;
  Vector pos;
  Vector front;
  Vector up;
  void *priv;
  NormalFunc normal_func;
  IntersectFunc intersection;
  AttenuationFunc reflective_attenuation_func;
  RefractionFunc refraction_func;

} Object;

Ray get_reflection_by_normal_and_ray(const Ray *ray, const Vector *normal, const Vector *intersection);
#endif //RAYTRACING_OBJECT_H
