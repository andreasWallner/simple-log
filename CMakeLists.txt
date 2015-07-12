cmake_minimum_required(VERSION 2.6)

project(LOGLIB)

if(CMAKE_SOURCE_DIR STREQUAL LOGLIB_SOURCE_DIR)
  include("flags.cmake")
endif(CMAKE_SOURCE_DIR STREQUAL LOGLIB_SOURCE_DIR)

# Building of tests is optional
option(BUILD_TESTS "Switch to enable/disable building of tests." false)

set(sources
   Log.cpp
   LogWriter.cpp)

if(BUILD_TESTS)
   enable_testing()
   add_subdirectory(tests)
endif(BUILD_TESTS)

source_group(log_src FILES ${sources})
add_library(log STATIC ${sources})
