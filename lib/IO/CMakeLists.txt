add_library(FileIO fileReader.cpp fileWriter.cpp)

install(TARGETS FileIO
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)