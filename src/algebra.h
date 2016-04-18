//
// Created by alexwang on 4/18/16.
//

#ifndef RAYTRACING_ALGEBRA_H
#define RAYTRACING_ALGEBRA_H

typedef struct {
  double x, y, z;
} vector;
void vector_init(vector *v);

double modulation(const vector *v);
double modulation2(const vector *v);
double dot(const vector *a, const vector *b);
vector cross(const vector *a, const vector *b);
void normalize(vector *v);
vector sub(const vector *a, const vector *b);
vector add(const vector *a, const vector *b);
vector rmul(const vector *a, double v);

typedef struct {
  vector pos;
  vector front;
} Ray;

#endif