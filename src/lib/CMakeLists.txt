find_package(sdl2)
if (${sdl2_FOUND})
else ()
endif()

target_include_directories(ignis
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/cglm/include/
)
