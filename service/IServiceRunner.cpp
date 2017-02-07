#include "IServiceRunner.h"

IServiceRunner::IServiceRunner(ILog* log)
        : p_log(log) {}

IServiceRunner::~IServiceRunner() {
    if(!this->p_log)
        delete this->p_log;
}

#ifdef _WIN32

int IServiceRunner::Init(DWORD argc, LPTSTR* argv) {
    this->getLog()->writeLine("Init Arguments: ");
    for(DWORD i = 0; i < argc; i++)
        this->getLog()->writeLine("i || " + std::string(argv[i]));

    return SERVICE_NO_ERROR;
}

void IServiceRunner::OnUserControl(DWORD control) {
    this->getLog()->writeLine("OnUserControl: " + std::to_string(control));
}

#endif

int IServiceRunner::OnPause() {
    return SERVICE_NO_ERROR;
}

int IServiceRunner::OnContinue() {
    return SERVICE_NO_ERROR;
}

void IServiceRunner::OnStop() {}

void IServiceRunner::OnShutdown() {
    this->OnStop();
}
void IServiceRunner::OnInquire() {}

void IServiceRunner::Install() {}

void IServiceRunner::UnInstall() {}

ILog* IServiceRunner::getLog() {
    if(!this->p_log)
        this->p_log = new ILog();

    return this->p_log;
}