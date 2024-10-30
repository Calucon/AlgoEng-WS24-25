add_library(AESort mergeSort.cpp)

install(TARGETS AESort
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)