cmake_minimum_required(VERSION 3.0)
project(raytracing)

set(SOURCE_FILES
        src/main.c
        src/camera.c
        src/bitmap.c
        src/algebra.c
        src/config.c
        src/light.c
        src/world.c
        src/list.c
        src/sphere.c
        src/plane.c
        src/object.c
        src/threadpool/thpool.c src/ray_trace.c src/ray_trace.h)
add_executable(raytracing ${SOURCE_FILES})
target_link_libraries(raytracing m)
target_link_libraries(raytracing png)
target_link_libraries(raytracing yaml)
target_link_libraries(raytracing pthread)