function(copy_dlls target)
  if(WIN32)
    add_custom_command(
      TARGET "${target}"
      POST_BUILD
      COMMAND "${CMAKE_COMMAND}" -E copy -t "$<TARGET_FILE_DIR:${target}>"
              "$<TARGET_RUNTIME_DLLS:${target}>"
      USES_TERMINAL COMMAND_EXPAND_LISTS)
  endif()
endfunction()
