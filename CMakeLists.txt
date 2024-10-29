cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(AlgoEngPKSS)
#configure_file(AlgoEngPKSS.h.in AlgoEngPKSS.h)
#target_include_directories(AlgoEngPKSS PUBLIC "${PROJECT_BINARY_DIR}")

add_executable(AlgoEngPKSS file_generator.cpp)