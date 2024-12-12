cmake_minimum_required(VERSION 3.21)

project(boost-asio-examples)

add_definitions("-D_WIN32_WINNT=0x0601")

if(${BUILD_COVERAGE})
  set(CLANG_COVERAGE_COMPILE_FLAGS
      "-fprofile-instr-generate -fcoverage-mapping")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CLANG_COVERAGE_COMPILE_FLAGS}")
endif()

execute_process(
  COMMAND
    ${CMAKE_COMMAND} -E create_symlink
    ${CMAKE_BINARY_DIR}/compile_commands.json
    ${CMAKE_SOURCE_DIR}/compile_commands.json)

enable_testing()

add_compile_definitions(PUBLIC $<$<CONFIG:Debug>:DEBUG>)

add_subdirectory("demos")
