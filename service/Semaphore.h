#ifndef SERVICE_SEMAPHORE_H
#define SERVICE_SEMAPHORE_H

#include <mutex>
#include <thread>

namespace service {

    class Semaphore {
    private:
        std::timed_mutex semaphore_mutex;
        bool isSemaphoreAcquired = false;

        /**
         * get the actual time as milliseconds
         * @return the time as milliseconds since epoch
         */
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

        /**
         * acquire the semaphore
         */
        void acquire() {
            this->semaphore_mutex.lock();
            this->isSemaphoreAcquired = true;
        }

        /**
         * try to acquire the semaphore
         * @return true - if acquire was successful / false - if not
         */
        bool tryAcquire() {
            if (this->semaphore_mutex.try_lock()) {
                this->isSemaphoreAcquired = true;
                return true;
            }

            return false;
        }

        /**
         * try to acquire the semaphore
         * @param timeoutMilliseconds the time to wait for the acquire
         * @return true - if acquire was successful / false - if not
         */
        bool tryAcquire(long long timeoutMilliseconds) {
            return this->tryAcquire(timeoutMilliseconds, (timeoutMilliseconds / 10));
        }

        /**
         * try to acquire the semaphore
         * @param timeoutMilliseconds the time to wait for the acquire
         * @param sleeptimeMillis the time to wait between the tries
         * @return true - if acquire was successful / false - if not
         */
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

        /**
         * release the semaphore
         */
        void release() {
            if (this->isSemaphoreAcquired) {
                this->isSemaphoreAcquired = false;
                this->semaphore_mutex.unlock();
            }
        }

        /**
         * check if the semaphore is acquired
         * @return true - if acquired / false - if not
         */
        bool isAcquired() {
            return this->isSemaphoreAcquired;
        }
    };

}

#endif //SERVICE_SEMAPHORE_H
