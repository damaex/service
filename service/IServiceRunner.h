#ifndef SERVICE_ISERVICERUNNER_H
#define SERVICE_ISERVICERUNNER_H

#include "ILog.h"

#ifdef _WIN32
#include <Windows.h>
#endif

class IServiceRunner {
private:
    ILog* p_log = NULL;
public:
    IServiceRunner(ILog* log);
    virtual ~IServiceRunner();

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

    ILog* getLog();
};

#endif //SERVICE_ISERVICERUNNER_H
