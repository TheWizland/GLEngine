cmake_minimum_required(VERSION 3.6)

project(
  GLEngine
  VERSION 1.0
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE SOURCE_FILES
    "Engine/*.h"
    "Engine/*.cpp")

if (UNIX)
	add_executable(source ${SOURCE_FILES})
	
	find_package(glfw3 3.3 REQUIRED)
	find_package(OpenGL REQUIRED)
	find_package(GLEW REQUIRED)
	find_package(glm CONFIG REQUIRED)

	target_include_directories(source PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/)
	target_link_libraries(source PRIVATE glfw glm::glm OpenGL::GL GLEW)
elseif (WIN32)	
	add_executable(source ${SOURCE_FILES})
	
	target_include_directories(source PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/)
	target_link_directories(source PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/libs/)
	target_link_libraries(source PRIVATE opengl32.lib glfw3.lib glew32.lib)
	
	#Copy glew32.dll to appropriate location.
	add_custom_command(TARGET source POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${PROJECT_SOURCE_DIR}/libs/glew32.dll"
        $<TARGET_FILE_DIR:source>)
	
	#Set startup project to source
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT source) 
endif()
