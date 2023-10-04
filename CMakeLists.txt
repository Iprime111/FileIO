cmake_minimum_required (VERSION 3.8 FATAL_ERROR)

project (FileIO)

if (NOT TARGET FileIO)

add_library (FileIO STATIC src/FileIO.cpp
                           headers/FileIO.h
                           headers/TextTypes.h)

target_include_directories (FileIO PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/headers)

add_subdirectory (CustomAssert)
target_link_libraries (FileIO PRIVATE CustomAssert)

endif ()
