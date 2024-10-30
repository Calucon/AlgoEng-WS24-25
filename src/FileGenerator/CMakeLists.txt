add_executable(FileGenerator main.cpp)

include_directories(${CMAKE_SOURCE_DIR}/lib/IO) # include files from FileReader
target_link_libraries(FileGenerator FileIO)

install(TARGETS FileGenerator RUNTIME DESTINATION bin)