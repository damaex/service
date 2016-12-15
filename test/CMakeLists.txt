set(SOURCE_FILES
        main.cpp)

find_package (Threads)

include_directories(${CMAKE_SOURCE_DIR}/service)
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static -lpthread")

add_executable(serviceTest ${SOURCE_FILES})
target_link_libraries(serviceTest service)
