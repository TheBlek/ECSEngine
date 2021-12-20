cmake_minimum_required(VERSION 3.16)

project(ECSEngine VERSION 0.1)

add_subdirectory(libs)

add_library(ECSEngine entity.cpp)
target_link_libraries(ECSEngine PUBLIC misc_libs)
target_include_directories(ECSEngine PUBLIC libs PUBLIC ${PROJECT_SOURCE_DIR})