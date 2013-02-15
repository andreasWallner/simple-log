cmake_minimum_required(VERSION 2.6)

SET (C_WARNINGS "-Wall -Wextra -Winit-self -pedantic -Wparentheses -Wfloat-equal -Wno-endif-labels -Wpointer-arith -Wdeprecated")
SET (C_WARNINGS "${C_WARNINGS} -Wmissing-declarations -Wredundant-decls -Wswitch-default -Wswitch-enum -Wsign-promo -Wlogical-op")
SET (C_WARNINGS "${C_WARNINGS} -Wconversion")
SET (C_WARNINGS "${C_WARNINGS} -Wcast-qual -Wmissing-include-dirs" CACHE STRING "Flags to enable warnings for C file compiles")
SET (CXX_WARNINGS "-Wsign-promo -Woverloaded-virtual ${C_WARNINGS}" CACHE STRING "Flags to enable warning for c++ file compiles")

SET (OPTIMIZE_C_FLAGS "-O2 -fivopts -ftree-loop-linear -fvisibility=hidden" CACHE STRING "Flags used to optimize C files")
SET (SHARED_C_FLAGS "-pipe -fno-ident -fvisibility-inlines-hidden" CACHE STRING "Flags shared for all C file compiles")
SET (SHARED_CXX_FLAGS "-std=c++0x" CACHE STRING "Flags shared for all C++ file compiles")
SET (SHARED_LD_FLAGS "-W1,-O1,--hash-style=gnu,--sort-common,--as-needed,--enable-new-dtags" CACHE STRING "Flags shared for all linker runs")

SET (CMAKE_C_FLAGS "${SHARED_C_FLAGS} ${C_WARNINGS}" CACHE STRING "Flags used for compiling C Files")
SET (CMAKE_CXX_FLAGS "${SHARED_CXX_FLAGS} ${SHARED_C_FLAGS} ${C_WARNINGS} ${CXX_WARNINGS}")
SET (CMAKE_LD_FLAGS	"${SHARED_LD_FLAGS}")

SET (CMAKE_C_FLAGS_DEBUG "-g ${CMAKE_C_FLAGS} -DDEBUG" CACHE STRING "C flags for C builds")
SET (CMAKE_CXX_FLAGS_DEBUG "-g ${CMAKE_CXX_FLAGS} -DDEBUG")
SET (CMAKE_LD_FLAGS_DEBUG "${SHARED_LD_FLAGS}")

SET (CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} ${OPTIMIZE_C_FLAGS} -DNDEBUG")
SET (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} ${OPTIMIZE_C_FLAGS} -DNDEBUG")
SET (CMAKE_LD_FLAGS_RELEASE "-s ${CMAKE_LD_FLAGS}")

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
