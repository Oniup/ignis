include(../../build_files/util.cmake)

add_executable(tests)
target_src_files(tests "tests")

target_include_directories(tests
   PUBLIC
      ${CMAKE_CURRENT_SOURCE_DIR}/
      ${ignis_INCLUDE_DIRS}
)
target_link_libraries(tests
    PRIVATE
        ignis
)

