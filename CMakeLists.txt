cmake_minimum_required(VERSION 3.0)
project(LSLOpenVR VERSION 0.0.1)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

# Step into the subdirectories
add_subdirectory(src)