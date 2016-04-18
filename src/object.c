//
// Created by alexwang on 4/18/16.
//
#include <memory.h>
#include <stdio.h>
#include <math.h>
#include "object.h"
#include "algebra.h"

int ball_intersect(const Ball *ball, const Ray *ray, vector *inter, vector *n) {

  vector sub_result = sub(&ray->pos, &ball->object.pos);
  // nearest point
  double t = -dot(&sub_result, &ray->front) / modulation2(&ray->front);

  vector v = rmul(&ray->front, t);
  vector nearest_point = add(&ray->pos, &v);

  vector nearest_v = sub(&nearest_point, &ball->object.pos);

  // nearest distance
  double nearest_dis2 = modulation2(&nearest_v);
  if (nearest_dis2 <= ball->radius * ball->radius) {
    double dis_nearest_to_inter = sqrt(ball->radius * ball->radius - nearest_dis2);
    vector delta;

    vector ray_direction = ray->front;
    normalize(&ray_direction);
    delta = rmul(&ray_direction, dis_nearest_to_inter);
    *inter = sub(&nearest_point, &delta);
    *n = sub(inter, &ball->object.pos);
    return 1;
  }
  else {
    return 0;
  }
}

void ball_init(Ball *ball) {
  memset(&ball->object, 0, sizeof(Object));
  vector_init(&ball->object.pos);
  vector_init(&ball->object.front);
  vector_init(&ball->object.up);
  ball->object.priv = ball;

  ball->object.pos.x = 2.0;
  ball->object.type = OT_Ball;
  ball->radius = 0.3;
}


