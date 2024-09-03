if (${BUILD_LIBRARY})
  add_subdirectory(ignis)
  add_subdirectory(lib)

  if (${BUILD_TESTS})
    add_subdirectory(tests)
  endif()
endif()

if (${BUILD_APP})
  add_subdirectory(app)
endif()

