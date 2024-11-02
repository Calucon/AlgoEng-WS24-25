add_library(AESort mergeSort.cpp introSort.cpp)
set_property(TARGET AESort PROPERTY CXX_STANDARD 23)

install(TARGETS AESort
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)