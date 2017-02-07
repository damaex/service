#ifndef TEST_LOG_H
#define TEST_LOG_H

#include <fstream>
#include "ILog.h"

class Log : public ILog {
private:
    std::ofstream   p_out;

    std::string     p_logFolderName;
    std::string     p_fileName;
    std::string     p_lastDate;
    std::string     p_fileEnding;

    void createLogsFolder();

    std::string getLogFolder();

    bool logFolderExists();

    bool logFileExists(std::string date);

    void checkLogFile();

    std::string getPathDelimeter();

    std::string getLogFilePathForDate(std::string date);

    std::string getExecutablePath();

    void writeOut(std::string text);

public:
    Log();
};

#endif //TEST_LOG_H
