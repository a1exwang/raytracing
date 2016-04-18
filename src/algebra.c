//
// Created by alexwang on 4/18/16.
//

#include "algebra.h"
#include <math.h>

void vector_init(vector *v) {
  v->x = v->y = v->z = 0;
}


double modulation(const vector *v) {
  return sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
}

double modulation2(const vector *v) {
  return v->x * v->x + v->y * v->y + v->z * v->z;
}

double dot(const vector *a, const vector *b) {
  return a->x * b->x + a->y * b->y + a->z * b->z;
}

vector sub(const vector *a, const vector *b) {
  vector result;
  result.x = a->x - b->x;
  result.y = a->y - b->y;
  result.z = a->z - b->z;
  return result;
}

vector add(const vector *a, const vector *b) {
  vector result;
  result.x = a->x + b->x;
  result.y = a->y + b->y;
  result.z = a->z + b->z;
  return result;
}

vector rmul(const vector *a, double v) {
  vector result;
  result.x = a->x * v;
  result.y = a->y * v;
  result.z = a->z * v;
  return result;
}

vector cross(const vector *a, const vector *b) {
  vector result;
  result.x = a->y * b->z - a->z * b->y;
  result.y = a->z * b->x - a->x * b->z;
  result.z = a->x * b->y - a->y * b->x;
  return result;
}

void normalize(vector *v) {
  double r = modulation(v);
  *v = rmul(v, 1 / r);
}
















