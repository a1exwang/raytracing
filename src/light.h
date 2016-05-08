//
// Created by alexwang on 4/18/16.
//

#ifndef RAYTRACING_LIGHT_H
#define RAYTRACING_LIGHT_H

#include "algebra.h"
#include "list.h"
#include "world.h"

struct TWorld;
struct TLight;
typedef Vector (*LightDiffuseFunc) (const struct TLight *light, const struct TWorld *world, const Vector *pos, const Object *object);
typedef Vector (*LightReflectiveFunc) (const struct TLight *light, const struct TWorld *world, const Vector *pos, const Object *object, const Ray *ray);

enum {
  Light_Spot
};

typedef struct TLight {
  ListHead list;
  int type;
  Vector pos;
  Vector color;
  LightDiffuseFunc diffuse_func;
  LightReflectiveFunc reflective_func;
  void *priv;
} Light;

//typedef struct {
//  Light light;
//  Vector left_top;
//  Vector right_top;
//  Vector color;
//} SurfaceLight;

void spot_light_init(Light *light, double r, double g, double b);
Vector spot_light_diffuse_func(const Light *light, const struct TWorld *world, const Vector *pos, const Object *object);
Vector spot_light_reflective_func(const Light *light,
                                  const struct TWorld *world,
                                  const Vector *pos,
                                  const Object *object,
                                  const Ray *ray);

#endif //RAYTRACING_LIGHT_H
