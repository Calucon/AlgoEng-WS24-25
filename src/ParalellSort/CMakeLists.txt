add_executable(ParalellSort main.cpp)
set_property(TARGET ParalellSort PROPERTY CXX_STANDARD 23)

include_directories(${CMAKE_SOURCE_DIR}/lib/Utils)
target_link_libraries(ParalellSort Utils)

include_directories(${CMAKE_SOURCE_DIR}/lib/Sort)
target_link_libraries(ParalellSort AESort)

install(TARGETS ParalellSort RUNTIME DESTINATION bin)