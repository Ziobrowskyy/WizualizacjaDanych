cmake_minimum_required(VERSION 3.17)
project(lab1)

set(CMAKE_CXX_STANDARD 20)

# Including GLFW
add_subdirectory(glfw-3.3.4)
include_directories(glfw-3.3.4/include)

include_directories(glad/include)

set(SOURCE_FILES main.cpp glad/src/glad.c)
add_executable(lab1 ${SOURCE_FILES})

# Linking GLFW
target_link_libraries(lab1 glfw ${GLFW_LIBRARIES})