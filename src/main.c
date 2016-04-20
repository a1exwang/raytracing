#include "bitmap.h"
#include "camera.h"
#include "config.h"


int main() {

  // create my world
  World world;
  world_init(&world);
  world_zygote(&world);

  // create a camera
  Camera camera;
  camera_init(&camera, 0, 0);
  read_configuration("config/camera.yaml", &camera, &world);

  world_bind_camera(&world, &camera);

  // initialize a bitmap
  Bitmap bitmap;
  bitmap_init(&bitmap, camera.width, camera.height);

  // render and save to png
  camera_render(&camera, &bitmap);
  bitmap_to_png(&bitmap, "image.png");

  return 0;
}