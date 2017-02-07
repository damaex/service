cmake_minimum_required(VERSION 3.1)
project(service)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

if (CMAKE_CXX_COMPILER_ID MATCHES GNU)
    #set(CMAKE_CXX_FLAGS "-Wall -Wno-unknown-pragmas -Wno-sign-compare -Woverloaded-virtual -Wwrite-strings -Wno-unused")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra")
endif (CMAKE_CXX_COMPILER_ID MATCHES GNU)

add_subdirectory(service)
add_subdirectory(test)