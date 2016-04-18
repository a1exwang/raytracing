//
// Created by alexwang on 4/18/16.
//

#ifndef RAYTRACING_BITMAP_H
#define RAYTRACING_BITMAP_H

#include "algebra.h"

typedef struct {
  int width, height;
  vector **buffer;
} Bitmap;

void bitmap_init(Bitmap *bitmap, int w, int h);
void bitmap_to_png(Bitmap *bitmap, const char *path);

int rgb_color_v(const vector *v);


#endif