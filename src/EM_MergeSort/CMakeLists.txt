add_executable(EM_MergeSort main.cpp)

include_directories(${CMAKE_SOURCE_DIR}/lib/IO)
target_link_libraries(EM_MergeSort FileIO)

include_directories(${CMAKE_SOURCE_DIR}/lib/Sort)
target_link_libraries(EM_MergeSort AESort)

install(TARGETS EM_MergeSort RUNTIME DESTINATION bin)