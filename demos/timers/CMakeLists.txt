cmake_minimum_required(VERSION 3.21)

project(timers)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory("synchronous")
add_subdirectory("asynchronous")
add_subdirectory("repeating")
add_subdirectory("multithread")
