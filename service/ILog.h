#ifndef SERVICE_ILOG_H
#define SERVICE_ILOG_H

#include <string>
#include "Semaphore.h"

#define SERVICE_NO_ERROR    0
#define SERVICE_ERROR       1

class ILog {
private:
    Semaphore p_semaphore;

protected:
    virtual void writeOut(std::string text);

public:
    virtual ~ILog() {}

    std::string getNewLine();
    std::string getCurrentTimeAndDate();
    std::string getCurrentDate();
    std::string getCurrentTime(std::string mod);

    void writeLine(std::string text);
    void write(std::string text);

    void writeError(std::string className, std::string functionName, std::string errorText);
    void writeError(std::string className, std::string functionName, int errorCode);
    void writeError(std::string className, std::string functionName, std::exception& e);
};

#endif //SERVICE_ILOG_H
