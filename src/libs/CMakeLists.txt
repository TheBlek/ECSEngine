cmake_minimum_required(VERSION 3.16)

project(ECSlibs VERSION 0.1)

add_compile_options(-std=c++17 -Wall)
set(CMAKE_BUILD_TYPE Debug)

configure_file(config.hpp.in generated/config.hpp)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

find_package(Boost REQUIRED COMPONENTS filesystem iostreams)

add_library(misc_libs vectors.cpp logger.cpp profiler.cpp renderer.cpp)
add_executable(test render.cpp)

target_include_directories(misc_libs PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/generated ${CMAKE_CURRENT_SOURCE_DIR}/../core ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(test PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/generated)

target_link_libraries(test PUBLIC glfw OpenGL::GL ${GLEW_LIBRARIES} Boost::filesystem Boost::iostreams)
target_link_libraries(misc_libs PUBLIC glfw PUBLIC OpenGL::GL PUBLIC ${GLEW_LIBRARIES} PUBLIC Boost::filesystem PUBLIC Boost::iostreams)
