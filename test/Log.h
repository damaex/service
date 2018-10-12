#ifndef TEST_LOG_H
#define TEST_LOG_H

#include <fstream>
#include <sys/stat.h>
#include <iostream>

#ifdef _WIN32
#include <Windows.h>
#endif

#include "ILog.h"

class Log : public service::ILog {
private:
    std::ofstream p_out;

    std::string p_logFolderName;
    std::string p_fileName;
    std::string p_lastDate;
    std::string p_fileEnding;

    void createLogsFolder() {
        if (!this->logFolderExists()) {
            std::cout << "log folder not available: create" << std::endl;
#ifdef _WIN32
            if (!CreateDirectory(this->getLogFolder().c_str(), NULL)) {
#else
            if (mkdir(this->getLogFolder().c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1) {
#endif
                std::cout << "Log Folder could not be created" << std::endl;
            }
            }
        }

    std::string getLogFolder() {
        return this->getExecutablePath() + this->getPathDelimeter() + this->p_logFolderName + this->getPathDelimeter();
    }

    bool logFolderExists() {
        struct stat info;
        bool ret = false;

        if (stat(this->getLogFolder().c_str(), &info) != 0) {
            std::cout << "cannot access " << this->getLogFolder() << std::endl;
        }
        else if (info.st_mode & S_IFDIR) {  // S_ISDIR() doesn't exist on my windows
            ret = true;
        }
        else {
            std::cout << this->getLogFolder() << " is no directory" << std::endl;
        }

        return ret;
    }

    bool logFileExists(const std::string &date) {
        std::string path = this->getLogFilePathForDate(date);

        struct stat buf;
        return (stat(path.c_str(), &buf) == 0);
    }

    void checkLogFile() {
        if (this->p_fileName.empty()) {
            // restart
            this->p_lastDate = this->getCurrentDate();
            int i = 0;

            if (this->logFileExists(this->p_lastDate)) {
                while (true) {
                    if (this->logFileExists(this->p_lastDate + " - " + std::to_string(i))) {
                        i++;
                    }
                    else {
                        this->p_fileName = this->p_lastDate + " - " + std::to_string(i);
                        break;
                    }
                }
            }
            else {
                this->p_fileName = this->p_lastDate;
            }

            this->p_out.open(this->getLogFilePathForDate(this->p_fileName), std::ios::app);

        }
        else {

            //check while running for new day
            if (this->p_lastDate != this->getCurrentDate()) {
                this->p_out.close();

                this->p_lastDate = this->getCurrentDate();
                this->p_fileName = this->p_lastDate;

                this->p_out.open(this->getLogFilePathForDate(this->p_fileName), std::ios::app);
            }

        }
        //TODO: configurable how many log files get stored
    }

    std::string getPathDelimeter() {
#ifdef _WIN32
        return "\\";
#else
        return "/";
#endif
    }

    std::string getLogFilePathForDate(const std::string &date) {
        return this->getLogFolder() + date + "." + this->p_fileEnding;
    }

    std::string getExecutablePath() {
#ifdef _WIN32
        HMODULE hModule = GetModuleHandleW(NULL);
        WCHAR path[MAX_PATH];
        GetModuleFileNameW(hModule, path, MAX_PATH);

        std::wstring ws(path);
        std::string full(ws.begin(), ws.end());

        return full.substr(0, full.find_last_of("\\/"));
#else
        //TODO unix
        return "";
#endif
    }

    void writeOut(const std::string &text) override {
        this->createLogsFolder();
        this->checkLogFile();

        this->p_out << text;
        this->p_out.flush();
    }

public:
    Log() {
        this->checkLogFile();
    }
};

#endif //TEST_LOG_H
