cmake_minimum_required(VERSION 3.2)
project(service)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (CMAKE_CXX_COMPILER_ID MATCHES GNU)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra")
endif (CMAKE_CXX_COMPILER_ID MATCHES GNU)

add_subdirectory(test)