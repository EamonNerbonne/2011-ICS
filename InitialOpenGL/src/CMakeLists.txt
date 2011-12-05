cmake_minimum_required(VERSION 2.8)
project(SampleOpenGL)

#find_package(GLUT)
#find_package(OpenGL)

include("optimization-options.cmake")

#include_directories($ENV{BOOST_DIR})
#message("Using Boost dir: $ENV{BOOST_DIR}")
include_directories("../../eigen")
include_directories("/usr/include")

#set(CMAKE_INCLUDE_CURRENT_DIR ON) 

set(sources sample.cpp InitOpenGL.cpp RotationState.cpp RgbaImageTexture.cpp stb_image.c)
add_executable(sample ${sources})
target_link_libraries(sample m X11 Xi Xmu glut GL GLU)

