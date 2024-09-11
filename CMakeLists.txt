cmake_minimum_required(VERSION 3.20)

set(VERSION_MAJOR 0 CACHE INTERNAL "")
set(VERSION_MINOR 0 CACHE INTERNAL "")
set(VERSION_PATCH 1 CACHE INTERNAL "")
set(VERSION_STR "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}"  CACHE INTERNAL "")

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED 11)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/binaries)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/binaries)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/binaries)

project(Ignis
  LANGUAGES C
  VERSION ${VERSION_STR}
)

option(BUILD_APP "Build game editor (.exe)" ON)

option(BUILD_LIBRARY "Build a static (.lib) runtime library" ON)
option(BUILD_TESTS "Build runtime tests" OFF)
option(TRACE_MEMORY_STATS "Track memory allocations count and total size in bytes" ON)

add_subdirectory(src)
