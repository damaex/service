#include "Log.h"

bool Log::logFolderExists() {
    //TODO
}

void Log::createLogFolder() {
    if(!this->logFolderExists()) {
        //TODO
    }
}

std::string Log::getDateTimeString() {

}

std::string Log::getNewLine() {
#ifdef _WIN32
    return "\r\n";
#else
    return "\n";
#endif
}

void Log::checkLogFile() {
    this->createLogFolder();
}

Log::Log() {
    this->checkLogFile();
}

void Log::writeLine(std::string text) {
    this->write(text + this->getNewLine());
}

void Log::write(std::string text) {
    //TODO: Semaphore
    this->checkLogFile();
}

void Log::writeError(std::string className, std::string functionName, std::string errorText) {
    this->writeLine(this->getDateTimeString() + " || " + className + " :: " + functionName + " :: " + errorText);
}

void Log::writeError(std::string className, std::string functionName, int errorCode) {
    this->writeError(className, functionName, std::to_string(errorCode));
}

void Log::writeError(std::string className, std::string functionName, std::exception& e) {
    this->writeError(className, functionName, e.what());
}
