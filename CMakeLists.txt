cmake_minimum_required(VERSION 3.2)
project(wflood)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES wflood.cpp httpcpp.cpp)
add_executable(wflood ${SOURCE_FILES})