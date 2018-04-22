#include "Semaphore.h"
#include <thread>

Semaphore::~Semaphore() {
    if (this->isAquired())
        this->release();
}

long long Semaphore::getMilliseconds() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

void Semaphore::acquire() {
    this->sema.lock();
    this->isSemaAquired = true;
}

bool Semaphore::tryAcquire() {
    if (this->sema.try_lock()) {
        this->isSemaAquired = true;
        return true;
    } else {
        return false;
    }
}

bool Semaphore::tryAcquire(long long timeoutMilliseconds) {
    return this->tryAcquire(timeoutMilliseconds, (timeoutMilliseconds / 10));
}

bool Semaphore::tryAcquire(long long timeoutMilliseconds, long long sleeptimeMillis) {
    std::chrono::milliseconds duration(sleeptimeMillis);
    long long endtime = this->getMilliseconds() + timeoutMilliseconds;
    while (this->getMilliseconds() < endtime) {
        if (this->tryAcquire())
            return true;
        else
            std::this_thread::sleep_for(duration);
    }
    return this->tryAcquire();
}

void Semaphore::release() {
    if (this->isSemaAquired) {
        this->isSemaAquired = false;
        this->sema.unlock();
    }
}

bool Semaphore::isAquired() {
    return this->isSemaAquired;
}
