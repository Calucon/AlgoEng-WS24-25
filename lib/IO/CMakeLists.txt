add_library(FileIO fileReader.cpp fileWriter.cpp)
set_property(TARGET FileIO PROPERTY CXX_STANDARD 23)

install(TARGETS FileIO
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)