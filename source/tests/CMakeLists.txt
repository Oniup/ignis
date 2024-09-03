include(../../build_files/util.cmake)

add_executable(ignis_tests)

target_src_files(ignis_tests "framework")

target_include_directories(ignis_tests
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/
    ${ignis_INCLUDE_DIRS}
)

target_link_libraries(ignis_tests
  PRIVATE
    ignis
)
