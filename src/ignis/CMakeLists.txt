add_library(ignis STATIC)
add_subdirectory(ignis)

target_include_directories(ignis
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_definitions(ignis
    PUBLIC
        IGNIS_VERSION_MAJOR=${VERSION_MAJOR}
        IGNIS_VERSION_MINOR=${VERSION_MINOR}
        IGNIS_VERSION_PATCH=${VERSION_PATCH}
        IGNIS_VERSION_STR="${VERSION_STR}"
)

if (${TRACE_MEMORY_STATS})
    target_compile_definitions(ignis
        PUBLIC
            IGNIS_TRACE_MEMORY_STATS=${TRACE_MEMORY_STATS}
    )
endif()
