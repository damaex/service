#ifndef SERVICE_ILOG_H
#define SERVICE_ILOG_H

#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
#include "Semaphore.h"

#define SERVICE_NO_ERROR    0
#define SERVICE_ERROR       1

#if defined(__GNUC__) && !defined(__clang__)
#   include <features.h>
#   if __GNUC_PREREQ(5, 0)
#       define SERVICE_HAS_PUT_TIME    1
#   else
#       define SERVICE_HAS_PUT_TIME    0
#   endif
#else
#   if defined(__clang__)
#       if __clang_major__ >= 6
#           define SERVICE_HAS_PUT_TIME    1
#       else
#           define SERVICE_HAS_PUT_TIME    0
#       endif
#   else
#       define SERVICE_HAS_PUT_TIME    1
#   endif
#endif

namespace service {

    class ILog {
    private:
        Semaphore p_semaphore;

    protected:
        virtual void writeOut(const std::string &text) {
            std::cout << text;
        }

    public:
        /**
         * get system style new line string
         */
        std::string getNewLine() {
#ifdef _WIN32
            return "\r\n";
#else
            return "\n";
#endif
        }

        std::string getCurrentTimeAndDate() {
            return this->getCurrentTime("%Y-%m-%d %H:%M:%S");
        }

        std::string getCurrentDate() {
            return this->getCurrentTime("%Y-%m-%d");
        }

        std::string getCurrentTime(const std::string &mod) {
            auto now = std::chrono::system_clock::now();
            auto in_time_t = std::chrono::system_clock::to_time_t(now);

            struct tm timeinfo;
#ifdef _WIN32
            localtime_s(&timeinfo, &in_time_t);
#else
            timeinfo = *localtime(&in_time_t);
#endif

#if SERVICE_HAS_PUT_TIME == 1
            std::stringstream ss;
            ss << std::put_time(&timeinfo, mod.c_str());
            return ss.str();
#else
            char foo[24];

            if (0 < strftime(foo, sizeof(foo), mod.c_str(), &timeinfo))
                return std::string(foo);
            
            return "";
#endif
        }

        void writeLine(const std::string &text) {
            this->write(text + this->getNewLine());
        }

        void write(const std::string &text) {
            this->p_semaphore.acquire();
            this->writeOut(this->getCurrentTimeAndDate() + " || " + text);
            this->p_semaphore.release();
        }

        void writeError(const std::string &className, const std::string &functionName, const std::string &errorText) {
            this->writeLine(className + " :: " + functionName + " :: " + errorText);
        }

        void writeError(const std::string &className, const std::string &functionName, int errorCode) {
            this->writeError(className, functionName, std::to_string(errorCode));
        }

        void writeError(const std::string &className, const std::string &functionName, std::exception &e) {
            this->writeError(className, functionName, e.what());
        }
    };

}

#endif //SERVICE_ILOG_H
