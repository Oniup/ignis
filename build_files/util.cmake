function(target_src_files target dir_name)
  file(GLOB_RECURSE SOURCE_FILES
    RELATIVE
      ${CMAKE_CURRENT_SOURCE_DIR} "${dir_name}/*.c"
  )
  target_sources(${target}
    PRIVATE
      ${SOURCE_FILES}
  )
endfunction()

