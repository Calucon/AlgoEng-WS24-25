add_executable(ParallelSort main.cpp)
set_property(TARGET ParallelSort PROPERTY CXX_STANDARD 23)

include_directories(${CMAKE_SOURCE_DIR}/lib/Utils)
target_link_libraries(ParallelSort Utils)

include_directories(${CMAKE_SOURCE_DIR}/lib/Sort)
target_link_libraries(ParallelSort AESort)

install(TARGETS ParallelSort RUNTIME DESTINATION bin)