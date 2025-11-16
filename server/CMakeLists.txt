cmake_minimum_required(VERSION 3.10)
project(contactserver)

set(CMAKE_CXX_STANDARD 17)

add_executable(contactserver contactserver.cpp)
target_link_libraries(contactserver rt)

