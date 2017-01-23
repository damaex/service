#ifndef SERVICE_LOG_H
#define SERVICE_LOG_H

#define SERVICE_NO_ERROR    0
#define SERVICE_ERROR       1

#include <string>
#include <exception>

class Log {
private:
    bool logFolderExists();
    void createLogFolder();
    std::string getDateTimeString();
    std::string getNewLine();

    void checkLogFile();
public:
    Log();

    void writeLine(std::string text);
    void write(std::string text);

    void writeError(std::string className, std::string functionName, std::string errorText);
    void writeError(std::string className, std::string functionName, int errorCode);
    void writeError(std::string className, std::string functionName, std::exception& e);
};


#endif //SERVICE_LOG_H
