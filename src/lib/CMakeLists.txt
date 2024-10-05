option(GLFW_BUILD_EXAMPLES "Build the GLFW example programs" OFF)
option(GLFW_BUILD_TESTS "Build the GLFW test programs" OFF)
option(GLFW_BUILD_DOCS "Build the GLFW documentation" OFF)
option(GLFW_INSTALL "Generate installation target" OFF)

add_subdirectory(glfw)

target_sources(ignis
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/glad/src/glad.c
        ${CMAKE_CURRENT_SOURCE_DIR}/cthreads/CThreads/cthreads.c
)

target_include_directories(ignis
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/cglm/include/
        ${CMAKE_CURRENT_SOURCE_DIR}/cthreads/
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/stb/
        ${CMAKE_CURRENT_SOURCE_DIR}/glfw/include
        ${CMAKE_CURRENT_SOURCE_DIR}/glad/include
)

target_link_libraries(ignis
    PRIVATE
        glfw
)
