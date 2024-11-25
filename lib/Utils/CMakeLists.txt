add_library(Utils numberGen.cpp threadPool.cpp)
set_property(TARGET Utils PROPERTY CXX_STANDARD 23)

install(TARGETS Utils
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)