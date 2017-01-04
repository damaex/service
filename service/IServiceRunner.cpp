#include "IServiceRunner.h"

#if !defined(_WIN32)
#define NO_ERROR 0
#endif

#ifdef _WIN32

int IServiceRunner::Init(DWORD argc, LPTSTR* argv) { return NO_ERROR; }
void IServiceRunner::OnUserControl(DWORD control) {}

#endif

int IServiceRunner::OnPause() { return NO_ERROR; }
int IServiceRunner::OnContinue() { return NO_ERROR; }
void IServiceRunner::OnStop() {}
void IServiceRunner::OnShutdown() { this->OnStop(); }
void IServiceRunner::OnInquire() {}
void IServiceRunner::Install() {}
void IServiceRunner::UnInstall() {}
