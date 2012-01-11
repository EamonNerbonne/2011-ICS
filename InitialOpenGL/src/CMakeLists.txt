cmake_minimum_required(VERSION 2.8)
project(SampleOpenGL)

#find_package(GLUT)
#find_package(OpenGL)

include("optimization-options.cmake")

#include_directories($ENV{BOOST_DIR})
#message("Using Boost dir: $ENV{BOOST_DIR}")
include_directories("../../eigen")

if(CMAKE_COMPILER_IS_GNUCXX)
	include_directories("/usr/include")
else()
	include_directories("../../freeglut/include")
	link_directories ("../../freeglut/lib") 
endif()


#set(CMAKE_INCLUDE_CURRENT_DIR ON) 

set(sources sample.cpp InitOpenGL.cpp EulerRotation.cpp QuaternionRotation.cpp RotationModel.cpp BookRenderer.cpp RgbaImageTexture.cpp stb_image.c)
add_executable(sample ${sources})
if(CMAKE_COMPILER_IS_GNUCXX)
	target_link_libraries(sample m X11 Xi Xmu glut GL GLU)
elseif(MSVC)
	target_link_libraries(sample freeglut)
	ADD_CUSTOM_COMMAND(
	   TARGET sample
	   POST_BUILD
	   COMMAND ${CMAKE_COMMAND} -E copy "../../freeglut/freeglut.dll" "./freeglut.dll"
	)
else()
	message(FATAL_ERROR "Unrecognized compiler!")
endif()


