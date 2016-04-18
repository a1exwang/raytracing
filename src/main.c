#include "bitmap.h"
#include "camera.h"
#include "config.h"
int w = 1024;
int h = 768;

int main() {
  Camera camera;
  camera_init(&camera, w, h);
  //camera_goto(&camera, 0, 0, 0);
  //camera_ortho(&camera, -1, 1, -1, 1, -1, 1);
  //camera_viewport(&camera, 1, 0, 0, 0, 0, 1, 2, 2);
  camera_configure("config/camera.yaml", &camera);

  // initialize a bitmap
  Bitmap bitmap;
  bitmap_init(&bitmap, w, h);

  // render and save to png
  camera_shot(&camera, &bitmap);
  bitmap_to_png(&bitmap, "image.png");

  return 0;
}