add_executable(FileGenerator main.cpp)

include_directories(${CMAKE_SOURCE_DIR}/lib//FileReader) # include files from FileReader
target_link_libraries(FileGenerator FileReader)

install(TARGETS FileGenerator RUNTIME DESTINATION bin)