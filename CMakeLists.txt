cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)
#set(EXECUTABLE_OUTPUT_PATH "bin")

project(AlgoEngPKSS)

# libraries
add_subdirectory(lib/IO)

# programms
add_subdirectory(src/FileAnalyzer)
add_subdirectory(src/FileGenerator)
add_subdirectory(src/EM_MergeSort)