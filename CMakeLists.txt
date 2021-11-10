cmake_minimum_required(VERSION 3.17)
project(wizualizacja-danych)

set(CMAKE_CXX_STANDARD 20)

# Including GLFW
add_subdirectory(glfw-3.3.4)
include_directories(glfw-3.3.4/include)

include_directories(glad/include)
include_directories(glm)

set(LIB_FILES glad/src/glad.c)

add_executable(lab1 lab1.cpp ${LIB_FILES})
add_executable(lab2 lab2.cpp ${LIB_FILES})
add_executable(lab3 lab3.cpp ${LIB_FILES})
add_executable(lab4 lab4.cpp ${LIB_FILES})

# Linking GLFW
target_link_libraries(lab1 glfw ${GLFW_LIBRARIES})
target_link_libraries(lab2 glfw ${GLFW_LIBRARIES})
target_link_libraries(lab3 glfw ${GLFW_LIBRARIES})
target_link_libraries(lab4 glfw ${GLFW_LIBRARIES})