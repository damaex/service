cmake_minimum_required(VERSION 3.2)
project(service)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (CMAKE_CXX_COMPILER_ID MATCHES GNU)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra")
endif (CMAKE_CXX_COMPILER_ID MATCHES GNU)

find_package(Threads REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/service)

set(SERVICE_FILES
    ${CMAKE_SOURCE_DIR}/service/ILog.hpp
    ${CMAKE_SOURCE_DIR}/service/IServiceRunner.hpp
    ${CMAKE_SOURCE_DIR}/service/Semaphore.hpp
    ${CMAKE_SOURCE_DIR}/service/Service.hpp
    ${CMAKE_SOURCE_DIR}/service/ServiceHandler.hpp)

set(TEST_FILES
    ${CMAKE_SOURCE_DIR}/test/Log.hpp
    ${CMAKE_SOURCE_DIR}/test/main.cpp)

add_executable(serviceTest ${TEST_FILES} ${SERVICE_FILES})

source_group("service"            FILES ${SERVICE_FILES})
source_group("test"            FILES ${TEST_FILES})

target_link_libraries(serviceTest Threads::Threads)
