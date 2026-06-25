cmake_minimum_required(VERSION 3.10)
project(raytracer)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)

add_executable(raytracer
  src/main.cpp
  src/camera.cpp
  src/sphere.cpp
  src/hittable_list.cpp
  src/material.cpp
  src/stb_image_impl.cpp
)

target_include_directories(raytracer PRIVATE include)
