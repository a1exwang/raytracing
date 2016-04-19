#include "bitmap.h"
#include "camera.h"
#include "config.h"
int w = 1024;
int h = 768;

int main() {

  // create a camera
  Camera camera;
  camera_init(&camera, w, h);
  camera_configure("config/camera.yaml", &camera);

  // initialize the world
  World world;
  world_init(&world);
  world_zygote(&world);
  world_bind_camera(&world, &camera);

  // initialize a bitmap
  Bitmap bitmap;
  bitmap_init(&bitmap, w, h);

  // render and save to png
  camera_render(&camera, &bitmap);
  bitmap_to_png(&bitmap, "image.png");

  return 0;
}