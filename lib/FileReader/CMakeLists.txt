add_library(FileReader fileReader.cpp)

install(TARGETS FileReader
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)