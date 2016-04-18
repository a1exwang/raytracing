//
// Created by alexwang on 4/18/16.
//

#ifndef RAYTRACING_OBJECT_H
#define RAYTRACING_OBJECT_H

#include "algebra.h"

enum {
  OT_Ball
};

typedef struct {
  int type;
  vector pos;
  vector front;
  vector up;
  void *priv;
} Object;

typedef struct {
  Object object;
  double radius;
} Ball;


void ball_init(Ball *ball);
int ball_intersect(const Ball *ball, const Ray *ray, vector *inter, vector *n);




#endif //RAYTRACING_OBJECT_H
