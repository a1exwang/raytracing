//
// Created by alexwang on 4/18/16.
//

#ifndef RAYTRACING_OBJECT_H
#define RAYTRACING_OBJECT_H

#include "algebra.h"
#include "list.h"

enum {
  OT_Ball
};

struct TObject;

typedef Vector (*NormalFunc) (const struct TObject *object, const Vector *pos);
typedef int (*IntersectFunc)(const struct TObject *object, const Ray *ray, Ray *reflect, Vector *pt, Vector *n);
typedef Vector (*ColorFunc)(const struct TObject *object, const Ray *ray, Ray *reflect, Vector *pt, Vector *n);

typedef struct TObject {
  ListHead list;

  int type;
  Vector pos;
  Vector front;
  Vector up;
  void *priv;
  NormalFunc normal_func;
  IntersectFunc intersection;
  ColorFunc color;
} Object;


#endif //RAYTRACING_OBJECT_H
