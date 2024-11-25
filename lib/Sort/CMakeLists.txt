add_library(AESort mergeSort.cpp introSort.cpp)
set_property(TARGET AESort PROPERTY CXX_STANDARD 23)

# target_link_libraries(AESort BS_thread_pool)
target_link_libraries(AESort Utils)

install(TARGETS AESort
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)