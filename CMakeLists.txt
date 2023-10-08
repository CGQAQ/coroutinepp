cmake_minimum_required(VERSION 3.26)
project(coroutine)

set(CMAKE_CXX_STANDARD 23)

add_executable(coroutine main.cpp)
find_package(fmt CONFIG REQUIRED)
target_link_libraries(coroutine PRIVATE fmt::fmt)