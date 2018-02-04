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
    virtual void writeOut(const std::string &text);

public:
    virtual ~ILog() {}

    std::string getNewLine();
    std::string getCurrentTimeAndDate();
    std::string getCurrentDate();
    std::string getCurrentTime(const std::string &mod);

    void writeLine(const std::string &text);
    void write(const std::string &text);

    void writeError(const std::string &className, const std::string &functionName, const std::string &errorText);
    void writeError(const std::string &className, const std::string &functionName, int errorCode);
    void writeError(const std::string &className, const std::string &functionName, std::exception& e);
};

#endif //SERVICE_ILOG_H
