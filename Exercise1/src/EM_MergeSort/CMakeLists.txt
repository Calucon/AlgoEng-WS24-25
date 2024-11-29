add_executable(EM_MergeSort main.cpp)
set_property(TARGET EM_MergeSort PROPERTY CXX_STANDARD 23)

include_directories(${CMAKE_SOURCE_DIR}/lib/IO)
target_link_libraries(EM_MergeSort FileIO)

include_directories(${CMAKE_SOURCE_DIR}/lib/Sort)
target_link_libraries(EM_MergeSort AESort)

install(TARGETS EM_MergeSort RUNTIME DESTINATION bin)