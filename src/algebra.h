//
// Created by alexwang on 4/18/16.
//

#ifndef RAYTRACING_ALGEBRA_H
#define RAYTRACING_ALGEBRA_H

typedef struct {
  double x, y, z;
} Vector;
void vector_init(Vector *v);

double modulation(const Vector *v);
double modulation2(const Vector *v);
double dot(const Vector *a, const Vector *b);
Vector cross(const Vector *a, const Vector *b);
void normalize(Vector *v);
Vector sub(const Vector *a, const Vector *b);
Vector add(const Vector *a, const Vector *b);
Vector rmul(const Vector *a, double v);

typedef struct {
  Vector pos;
  Vector front;
} Ray;

Vector v3(double x, double y, double z);


#endif