set(service_FILES
        ILog.cpp ILog.h
        IServiceRunner.cpp IServiceRunner.h
        Semaphore.cpp Semaphore.h
        Service.cpp Service.h)

add_library(service STATIC ${service_FILES})