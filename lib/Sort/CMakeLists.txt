add_library(AESort mergeSort.cpp introSort.cpp)
set_property(TARGET AESort PROPERTY CXX_STANDARD 23)

CPMAddPackage(
    NAME BS_thread_pool
    GITHUB_REPOSITORY bshoshany/thread-pool
    VERSION 4.1.0)
add_library(BS_thread_pool INTERFACE)
target_include_directories(BS_thread_pool INTERFACE ${BS_thread_pool_SOURCE_DIR}/include)

install(TARGETS AESort
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)