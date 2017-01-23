#include "IServiceRunner.h"

#ifdef _WIN32

int IServiceRunner::Init(DWORD argc, LPTSTR* argv) { return SERVICE_NO_ERROR; }
void IServiceRunner::OnUserControl(DWORD control) {}

#endif

int IServiceRunner::OnPause() { return SERVICE_NO_ERROR; }
int IServiceRunner::OnContinue() { return SERVICE_NO_ERROR; }
void IServiceRunner::OnStop() {}
void IServiceRunner::OnShutdown() { this->OnStop(); }
void IServiceRunner::OnInquire() {}
void IServiceRunner::Install() {}
void IServiceRunner::UnInstall() {}

Log* IServiceRunner::getLog() { return &this->p_log; }