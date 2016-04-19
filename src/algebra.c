//
// Created by alexwang on 4/18/16.
//

#include "algebra.h"
#include "light.h"
#include <math.h>

Vector v3(double x, double y, double z){
  Vector ret;
  ret.x = x;
  ret.y = y;
  ret.z = z;
  return ret;
}
void vector_init(Vector *v) {
  v->x = v->y = v->z = 0;
}

double modulation(const Vector *v) {
  return sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
}

double modulation2(const Vector *v) {
  return v->x * v->x + v->y * v->y + v->z * v->z;
}

double dot(const Vector *a, const Vector *b) {
  return a->x * b->x + a->y * b->y + a->z * b->z;
}

Vector sub(const Vector *a, const Vector *b) {
  Vector result;
  result.x = a->x - b->x;
  result.y = a->y - b->y;
  result.z = a->z - b->z;
  return result;
}

Vector add(const Vector *a, const Vector *b) {
  Vector result;
  result.x = a->x + b->x;
  result.y = a->y + b->y;
  result.z = a->z + b->z;
  return result;
}

Vector rmul(const Vector *a, double v) {
  Vector result;
  result.x = a->x * v;
  result.y = a->y * v;
  result.z = a->z * v;
  return result;
}

Vector cross(const Vector *a, const Vector *b) {
  Vector result;
  result.x = a->y * b->z - a->z * b->y;
  result.y = a->z * b->x - a->x * b->z;
  result.z = a->x * b->y - a->y * b->x;
  return result;
}

void normalize(Vector *v) {
  double r = modulation(v);
  *v = rmul(v, 1 / r);
}



















