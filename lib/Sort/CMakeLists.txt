add_library(AESort mergeSort.cpp introsort.cpp)

install(TARGETS AESort
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)