cmake_minimum_required(VERSION 3.21)

project(${PROJECT_NAME}-repeating)

include(CopyDlls)

find_package(Boost REQUIRED COMPONENTS asio)
find_package(fmt CONFIG REQUIRED)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} "main.cpp")

target_link_libraries(${PROJECT_NAME} PRIVATE Boost::asio 
                                              fmt::fmt)
copy_dlls(${PROJECT_NAME})

if(WIN32)
  install(
    TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION bin)
else()
  install(
    TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)
endif()
