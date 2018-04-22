#ifndef SERVICE_ISERVICERUNNER_H
#define SERVICE_ISERVICERUNNER_H

#include "ILog.h"
#include <memory>

#ifdef _WIN32
#include <Windows.h>
#endif

class IServiceRunner {
private:
    std::shared_ptr<ILog> p_log = nullptr;
public:
    explicit IServiceRunner(std::shared_ptr<ILog> log);

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

    std::shared_ptr<ILog> getLog();
};

#endif //SERVICE_ISERVICERUNNER_H
