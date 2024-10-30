add_library(MergeSort mergeSort.cpp)

install(TARGETS MergeSort
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)