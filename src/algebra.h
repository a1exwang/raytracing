//
// Created by alexwang on 4/18/16.
//

#ifndef RAYTRACING_ALGEBRA_H
#define RAYTRACING_ALGEBRA_H

#define ZERO 0.00001

typedef struct {
  double x, y, z;
} Vector;
void vector_init(Vector *v);

double modulation(const Vector *v);
double modulation2(const Vector *v);
double dot(const Vector *a, const Vector *b);
double vdot(Vector, Vector);
Vector cross(const Vector *a, const Vector *b);
Vector vcross(Vector, Vector);
void normalize(Vector *v);
Vector sub(const Vector *a, const Vector *b);
Vector add(const Vector *a, const Vector *b);
Vector vadd(Vector, Vector);
Vector rmul(const Vector *a, double v);
Vector vrmul(Vector, double);
int same_vector(const Vector *a, const Vector *b);

typedef struct {
  Vector pos;
  Vector front;
} Ray;

// pos must on ray
double ray_distance(const Ray *ray, const Vector *pos, int *success);
int zero(double value);
Vector v3(double x, double y, double z);
Vector color_black();
Vector color_mix(Vector a, Vector b);


#endif