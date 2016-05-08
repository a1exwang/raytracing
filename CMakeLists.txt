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

IF(CMAKE_BUILD_TYPE MATCHES DEBUG)
    message("debug mode")
    set(GCC_PROFILER_FLAGS -pg)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_PROFILER_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_PROFILER_FLAGS}")
ENDIF(CMAKE_BUILD_TYPE MATCHES DEBUG)

add_executable(raytracing ${SOURCE_FILES})
target_link_libraries(raytracing m)
target_link_libraries(raytracing png)
target_link_libraries(raytracing yaml)
target_link_libraries(raytracing pthread)