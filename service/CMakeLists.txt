set(service_FILES
        Service.cpp Service.h
        IServiceRunner.cpp IServiceRunner.h)

add_library(service STATIC ${service_FILES})