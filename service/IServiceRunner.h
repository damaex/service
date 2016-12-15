#ifndef ISERVICERUNNER_H
#define ISERVICERUNNER_H

#include <string>

#ifdef _WIN32
#include <Windows.h>
#endif

class IServiceRunner {
public:
    virtual std::string getName() = 0;
    virtual int Run() = 0;

    virtual int OnPause();
    virtual int OnContinue();

    virtual void OnStop();
    virtual void OnShutdown();

    virtual void OnInquire();

#ifdef _WIN32
    virtual void OnUserControl(DWORD control);
    virtual int Init(DWORD argc, LPTSTR* argv);
#endif

    virtual void Install();
    virtual void UnInstall();
};

#endif //ISERVICERUNNER_H
