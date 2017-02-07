#ifndef SERVICE_SEMAPHORE_H
#define SERVICE_SEMAPHORE_H

#include <mutex>

class Semaphore {
private:
    std::timed_mutex sema;
    bool isSemaAquired = false;

    long long getMilliseconds();

public:
    Semaphore();

    ~Semaphore();

    void acquire();

    bool tryAcquire();

    bool tryAcquire(long long time);

    bool tryAcquire(long long timeoutMilliseconds, long long sleeptimeMillis);

    void release();

    bool isAquired();
};

#endif //SERVICE_ISERVICERUNNER_H
