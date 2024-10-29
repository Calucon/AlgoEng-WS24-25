add_executable(MergeSort main.cpp)

include_directories(${CMAKE_SOURCE_DIR}/lib//FileReader) # include files from FileReader
target_link_libraries(MergeSort FileReader)

install(TARGETS MergeSort RUNTIME DESTINATION bin)