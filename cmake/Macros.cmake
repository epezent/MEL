macro(mel_source_group_by_folder target)
  set(SOURCE_GROUP_DELIMITER "/")
  set(last_dir "")
  set(files "")
  foreach(file ${${target}_SRC} ${${target}_HEADERS})
    file(RELATIVE_PATH relative_file "${PROJECT_SOURCE_DIR}/src/${target}" ${file})
    get_filename_component(dir "${relative_file}" PATH)
    if (NOT "${dir}" STREQUAL "${last_dir}")
      if (files)
        SOURCE_GROUP("${last_dir}" FILES ${files})
      endif (files)
      set(files "")
    endif (NOT "${dir}" STREQUAL "${last_dir}")
    set(files ${files} ${file})
    set(last_dir "${dir}")
  endforeach(file)
  if (files)
    source_group("${last_dir}" FILES ${files})
  endif (files)
endmacro(mel_source_group_by_folder)

macro(mel_incompatible first second)
  if (${first} AND ${second})
    message("Options " ${first} " and " ${second} " incompatible - disabling " ${second})
    set(${second} OFF)
  endif()
endmacro(mel_incompatible)

macro(mel_example MEL_EXAMPLE_NAME)
    add_executable(${MEL_EXAMPLE_NAME} "ex_${MEL_EXAMPLE_NAME}.cpp")
    target_link_libraries(${MEL_EXAMPLE_NAME} PRIVATE MEL::MEL)
    set_target_properties(${MEL_EXAMPLE_NAME} PROPERTIES FOLDER "Examples")
endmacro(mel_example)

macro(mel_test MEL_TEST_NAME)
    add_executable(${MEL_TEST_NAME} "test_${MEL_TEST_NAME}.cpp")
    target_link_libraries(${MEL_TEST_NAME} PRIVATE MEL::MEL)
    set_target_properties(${MEL_TEST_NAME} PROPERTIES FOLDER "Tests")
endmacro(mel_test)

