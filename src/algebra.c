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

int same_vector(const Vector *a, const Vector *b) {
  Vector delta = sub(a, b);
  return zero(modulation(&delta));
}
void normalize(Vector *v) {
  double r = modulation(v);
  *v = rmul(v, 1 / r);
}

int zero(double value) {
  return fabs(value) <= ZERO;
}

double ray_distance(const Ray *ray, const Vector *pos, int *success) {
  Vector delta = sub(&ray->pos, pos);
  Vector cross_value = cross(&delta, &ray->front);
  if (zero(modulation(&cross_value))) {
    *success = 1;
    return modulation(&delta);
  }
  else {
    *success = 0;
    return 0;
  }
}

Vector color_black() {
  Vector ret = { 0, 0, 0 };
  return ret;
}

void color_get_rgb(const Vector *color, double *r, double *g, double *b) {
//  *r = 1 - 1 / exp(color->x);
//  *g = 1 - 1 / exp(color->y);
//  *b = 1 - 1 / exp(color->z);
  *r = color->x;
  *g = color->y;
  *b = color->z;
}

Vector color_mix(Vector a, Vector b) {
  double e = M_E;
  Vector v = {
          .x = 1 - 1/pow(e, (log(1/(1-a.x)) + log(1/(1-b.x))) / log(e)),
          .y = 1 - 1/pow(e, (log(1/(1-a.y)) + log(1/(1-b.y))) / log(e)),
          .z = 1 - 1/pow(e, (log(1/(1-a.z)) + log(1/(1-b.z))) / log(e))
  };
//  Vector v = {
//          .x = fmax(a.x+b.x, 0.9999),
//          .y = fmax(a.y+b.y, 0.9999),
//          .z = fmax(a.z+b.z, 1)
//  };
//    Vector v = {
//          .x = fmax((a.x+b.x)/2, 1),
//          .y = fmax((a.y+b.y)/2, 1),
//          .z = fmax((a.z+b.z)/2, 1)
//  };
//  Vector v = {
//          a.x + b.x,
//          a.y + b.y,
//          a.z + b.z
//  };
  return v;
}



double vdot(Vector a, Vector b) {
  return dot(&a, &b);
}

Vector vcross(Vector a, Vector b) {
  Vector result = cross(&a, &b);
  return result;
}

Vector vadd(Vector a, Vector b) {
  Vector result = add(&a, &b);
  return result;
}

Vector vrmul(Vector a, double b) {
  Vector result = rmul(&a, b);
  return result;
}














