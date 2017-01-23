set(service_FILES
        IServiceRunner.cpp IServiceRunner.h
        Log.cpp Log.h
        Service.cpp Service.h)

add_library(service STATIC ${service_FILES})