#ifndef SERVICE_ILOG_H
#define SERVICE_ILOG_H

#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
#include "Semaphore.hpp"

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
        /**
         * write to the log
         * @param text the message to write
         */
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

        /**
         * get the current date and time
         * @return YYYY-MM-DD HH:MM:SS
         */
        std::string getCurrentTimeAndDate() {
            return this->getCurrentTime("%Y-%m-%d %H:%M:%S");
        }

        /**
         * get the current date
         * @return YYYY-MM-DD
         */
        std::string getCurrentDate() {
            return this->getCurrentTime("%Y-%m-%d");
        }

        /**
         * get the current time with modifier
         * @param mod the modifier with time and date
         * @return time and date as defined by modifier
         */
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

        /**
         * write message to log with line ending
         * @param text the message
         */
        void writeLine(const std::string &text) {
            this->write(text + this->getNewLine());
        }

        /**
         * write message to log
         * @param text the message
         */
        void write(const std::string &text) {
            this->p_semaphore.acquire();
            this->writeOut(this->getCurrentTimeAndDate() + " || " + text);
            this->p_semaphore.release();
        }

        /**
         * write error to log
         * @param className the class the error appeared in
         * @param functionName the function the error appeared in
         * @param errorText the error message
         */
        void writeError(const std::string &className, const std::string &functionName, const std::string &errorText) {
            this->writeLine(className + " :: " + functionName + " :: " + errorText);
        }

        /**
         * write error to log
         * @param className the class the error appeared in
         * @param functionName the function the error appeared in
         * @param errorCode the error code
         */
        void writeError(const std::string &className, const std::string &functionName, int errorCode) {
            this->writeError(className, functionName, std::to_string(errorCode));
        }

        /**
         * write error to log
         * @param className the class the error appeared in
         * @param functionName the function the error appeared in
         * @param e the exception
         */
        void writeError(const std::string &className, const std::string &functionName, std::exception &e) {
            this->writeError(className, functionName, e.what());
        }
    };

}

#endif //SERVICE_ILOG_H
