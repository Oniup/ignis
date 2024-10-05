if (${BUILD_LIBRARY})
    add_subdirectory(ignis)
    add_subdirectory(lib)

    if (${BUILD_TESTS})
        add_subdirectory(tests)
    endif()
endif()

if (${BUILD_EDITOR})
    add_subdirectory(editor)
endif()

