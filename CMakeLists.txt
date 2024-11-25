cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)
#set(EXECUTABLE_OUTPUT_PATH "bin")

project(AlgoEngPKSS)

# add dependencies
# include(cmake/CPM.cmake)

# install dependencies
#CPMAddPackage(
#    NAME BS_thread_pool
#    GITHUB_REPOSITORY bshoshany/thread-pool
#    VERSION 4.1.0)
#add_library(BS_thread_pool INTERFACE)
#target_include_directories(BS_thread_pool INTERFACE ${BS_thread_pool_SOURCE_DIR}/include

# libraries
add_subdirectory(lib/IO)
add_subdirectory(lib/Sort)
add_subdirectory(lib/Utils)

# programms
add_subdirectory(src/FileAnalyzer)
add_subdirectory(src/FileGenerator)
add_subdirectory(src/EM_MergeSort)
add_subdirectory(src/ParallelSort)