cmake_minimum_required(VERSION 3.5)
project(raytracing)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.c src/camera.c)
add_executable(raytracing ${SOURCE_FILES})