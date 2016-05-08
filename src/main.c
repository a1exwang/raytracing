#include <memory.h>
#include "bitmap.h"
#include "camera.h"
#include "config.h"


#define MAX_PATH_LEN 65536
int main(int argc, char **argv) {
  char *config_dir = (char *) malloc(MAX_PATH_LEN);
  if (argc == 2) {
    sprintf(config_dir, "%s/camera.yaml", argv[1]);
  }
  else {
    strcpy(config_dir, "/home/alexwang/dev/proj/graphics/raytracing/config/camera.yaml");
  }

  // create my world
  World world;
  world_init(&world);
  world_zygote(&world);

  // create a camera
  Camera camera;
  camera_init(&camera, 0, 0);
  read_configuration(config_dir, &camera, &world);

  world_bind_camera(&world, &camera);

  // initialize a bitmap
  Bitmap bitmap;
  bitmap_init(&bitmap, camera.width, camera.height);

  // render and save to png
  camera_render_sync(&camera, &bitmap);
  bitmap_to_png(&bitmap, "image.png");

  return 0;
}