#include "ILog.h"
#include <sstream>
#include <iomanip>
#include <iostream>

void ILog::writeOut(std::string text) {
    std::cout << text;
}

std::string ILog::getNewLine() {
#ifdef _WIN32
    return "\r\n";
#else
    return "\n";
#endif
}

std::string ILog::getCurrentTimeAndDate() {
    return  this->getCurrentTime("%Y-%m-%d %H:%M:%S");
}

std::string ILog::getCurrentDate() {
    return  this->getCurrentTime("%Y-%m-%d");
}

std::string ILog::getCurrentTime(std::string mod) {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), mod.c_str());
    return ss.str();
}

void ILog::writeLine(std::string text) {
    this->write(text + this->getNewLine());
}

void ILog::write(std::string text) {
    this->p_semaphore.acquire();
    this->writeOut(this->getCurrentTimeAndDate() + " || " + text);
    this->p_semaphore.release();
}

void ILog::writeError(std::string className, std::string functionName, std::string errorText) {
    this->writeLine(className + " :: " + functionName + " :: " + errorText);
}

void ILog::writeError(std::string className, std::string functionName, int errorCode) {
    this->writeError(className, functionName, std::to_string(errorCode));
}

void ILog::writeError(std::string className, std::string functionName, std::exception& e) {
    this->writeError(className, functionName, e.what());
}