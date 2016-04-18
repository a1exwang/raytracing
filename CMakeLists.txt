cmake_minimum_required(VERSION 3.5)
project(raytracing)

set(SOURCE_FILES src/main.c src/camera.c src/bitmap.c src/object.c src/algebra.c src/config.c src/config.h)
add_executable(raytracing ${SOURCE_FILES})
target_link_libraries(raytracing m)
target_link_libraries(raytracing png)
target_link_libraries(raytracing yaml)