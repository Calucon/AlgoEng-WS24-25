add_library(AESort mergeSort.cpp introSort.cpp)

install(TARGETS AESort
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)