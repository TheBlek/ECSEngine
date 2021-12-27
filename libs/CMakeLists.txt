cmake_minimum_required(VERSION 3.16)

project(ECSlibs VERSION 0.1)

add_compile_options(-std=c++17)
set(CMAKE_BUILD_TYPE Debug)

configure_file(config.hpp.in generated/config.hpp)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

add_library(misc_libs vector3.cpp logger.cpp render.cpp)
add_executable(test render.cpp)

target_include_directories(misc_libs PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/generated)
target_include_directories(test PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/generated)

target_link_libraries(test glfw OpenGL::GL ${GLEW_LIBRARIES})
target_link_libraries(misc_libs glfw OpenGL::GL ${GLEW_LIBRARIES})