set(SERVICE_FILES
    ${CMAKE_SOURCE_DIR}/service/ILog.h
    ${CMAKE_SOURCE_DIR}/service/IServiceRunner.h
    ${CMAKE_SOURCE_DIR}/service/Semaphore.h
    ${CMAKE_SOURCE_DIR}/service/Service.h
	${CMAKE_SOURCE_DIR}/service/ServiceHandler.h)

set(SOURCE_FILES
    Log.h
    main.cpp)

find_package(Threads)

include_directories(${CMAKE_SOURCE_DIR}/service)
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static -lpthread")
endif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")

add_executable(serviceTest ${SERVICE_FILES} ${SOURCE_FILES})
