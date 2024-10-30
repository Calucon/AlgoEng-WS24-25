add_executable(FileAnalyzer main.cpp)

include_directories(${CMAKE_SOURCE_DIR}/lib/IO) # include files from FileReader
target_link_libraries(FileAnalyzer FileIO)

install(TARGETS FileAnalyzer RUNTIME DESTINATION bin)