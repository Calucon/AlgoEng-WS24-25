add_library(Utils numberGen.cpp numberGen32.cpp threadPool.cpp semaphoreTracker.cpp)
set_property(TARGET Utils PROPERTY CXX_STANDARD 23)

install(TARGETS Utils
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)