#ifndef SERVICE_H
#define SERVICE_H

#include "IServiceRunner.h"

class Service {
private:
    IServiceRunner* p_runner;
    int             p_err;

    bool            p_isStarted;
    bool            p_isPaused;
    char            p_name[36];

#ifdef _WIN32
    LPSERVICE_MAIN_FUNCTION p_fpSrvMain;
    LPHANDLER_FUNCTION      p_fpSrvControl;

    SERVICE_TABLE_ENTRY     p_dispatchTable[2];
    SERVICE_STATUS          p_stat;
    SERVICE_STATUS_HANDLE   ph_stat;
#endif

public:
#ifdef _WIN32

    Service(IServiceRunner* runner, LPSERVICE_MAIN_FUNCTION funp_srvmain, LPHANDLER_FUNCTION funp_ctrl);

    int service(DWORD argc, LPTSTR* argv);
    void control(DWORD opcode);

    void ChangeStatus(DWORD state, DWORD checkpoint = (DWORD)0, DWORD waithint = (DWORD)0);

#else

    Service(IServiceRunner* runner);

#endif

    ~Service();

    bool install();
    bool uninstall();
    bool isInstalled();

    int start();

    int getExitCode();
    int getLastError();
};


#endif //SERVICE_H
