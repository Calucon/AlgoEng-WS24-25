add_executable(MergeSort main.cpp)

include_directories(${CMAKE_SOURCE_DIR}/lib/IO) # include files from FileReader
target_link_libraries(MergeSort FileIO)

install(TARGETS MergeSort RUNTIME DESTINATION bin)