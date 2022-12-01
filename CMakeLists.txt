cmake_minimum_required(VERSION 3.19)
project(Superficie)

set(CMAKE_CXX_STANDARD 14)

add_executable(Superficie basic_lighting_diffuse.cpp glad.c)

target_link_libraries(Superficie -lOpenGL32 -lGLU32 -lfreeGLUT -lglfw3dll)
