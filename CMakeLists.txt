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
    ${CMAKE_SOURCE_DIR}/service/ILog.h
    ${CMAKE_SOURCE_DIR}/service/IServiceRunner.h
    ${CMAKE_SOURCE_DIR}/service/Semaphore.h
    ${CMAKE_SOURCE_DIR}/service/Service.h
    ${CMAKE_SOURCE_DIR}/service/ServiceHandler.h)

set(TEST_FILES
    ${CMAKE_SOURCE_DIR}/test/Log.h
    ${CMAKE_SOURCE_DIR}/test/main.cpp)

add_executable(serviceTest ${TEST_FILES} ${SERVICE_FILES})

source_group("service"            FILES ${SERVICE_FILES})
source_group("test"            FILES ${TEST_FILES})

target_link_libraries(serviceTest
        Threads::Threads)