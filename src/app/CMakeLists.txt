include(../../build_files/util.cmake)

add_executable(ignis_editor)
target_src_files(ignis_editor "editor")

target_include_directories(ignis_editor
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/
    ${ignis_INCLUDE_DIRS}
)
target_link_libraries(ignis_editor
  PRIVATE
    ignis
)
