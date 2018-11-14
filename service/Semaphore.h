#ifndef SERVICE_SEMAPHORE_H
#define SERVICE_SEMAPHORE_H

#include <mutex>
#include <thread>

namespace service {

    class Semaphore {
    private:
        std::timed_mutex semaphore_mutex;
        bool isSemaphoreAcquired = false;

        long long getMilliseconds() {
            return std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::system_clock::now().time_since_epoch()).count();
        }

    public:
        Semaphore() = default;

        ~Semaphore() {
            if (this->isAcquired())
                this->release();
        }

        void acquire() {
            this->semaphore_mutex.lock();
            this->isSemaphoreAcquired = true;
        }

        bool tryAcquire() {
            if (this->semaphore_mutex.try_lock()) {
                this->isSemaphoreAcquired = true;
                return true;
            }

            return false;
        }

        bool tryAcquire(long long timeoutMilliseconds) {
            return this->tryAcquire(timeoutMilliseconds, (timeoutMilliseconds / 10));
        }

        bool tryAcquire(long long timeoutMilliseconds, long long sleeptimeMillis) {
            std::chrono::milliseconds duration(sleeptimeMillis);
            long long endtime = this->getMilliseconds() + timeoutMilliseconds;

            while (this->getMilliseconds() < endtime) {
                if (this->tryAcquire())
                    return true;

                std::this_thread::sleep_for(duration);
            }

            return this->tryAcquire();
        }

        void release() {
            if (this->isSemaphoreAcquired) {
                this->isSemaphoreAcquired = false;
                this->semaphore_mutex.unlock();
            }
        }

        bool isAcquired() {
            return this->isSemaphoreAcquired;
        }
    };

}

#endif //SERVICE_SEMAPHORE_H
