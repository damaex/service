find_package(Threads REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/service)

set(SERVICE_FILES
    ${CMAKE_SOURCE_DIR}/service/ILog.h
    ${CMAKE_SOURCE_DIR}/service/IServiceRunner.h
    ${CMAKE_SOURCE_DIR}/service/Semaphore.h
    ${CMAKE_SOURCE_DIR}/service/Service.h
    ${CMAKE_SOURCE_DIR}/service/ServiceHandler.h)

set(SOURCE_FILES
    Log.h
    main.cpp)

add_executable(serviceTest ${SERVICE_FILES} ${SOURCE_FILES})

source_group("service"            FILES ${SERVICE_FILES})

target_link_libraries(serviceTest
        Threads::Threads)
