cmake_minimum_required(VERSION 3.1...3.28)

project(
  GLEngine
  VERSION 1.0
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE SOURCE_FILES
    "Engine/*.h"
    "Engine/*.cpp")

add_executable(source ${SOURCE_FILES})

find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm CONFIG REQUIRED)

target_include_directories(source PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/)

target_link_libraries(source PRIVATE glfw glm::glm OpenGL::GL GLEW)
