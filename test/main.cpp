#include <iostream>
#include <thread>

#include "Service.h"

Service *service = NULL;

#ifdef _WIN32
void WINAPI serviceMain(DWORD ac, LPTSTR* av)
{
    service->service(ac, av);
}

void WINAPI serviceControl(DWORD opcode)
{
    service->control(opcode);
}
#endif

class Runner : public IServiceRunner {
private:
    bool p_running;
public:
    Runner() : p_running(false) {}

    std::string getName(){
        return "MyService";
    }

    int Run(){
        this->p_running = true;

        while (this->p_running){
            std::this_thread::sleep_for(std::chrono::seconds(3));
        }

        return 0;
    }

    void OnStop(){
        this->p_running = false;
    }
};

int main (int argc, char *argv[])
{
#ifdef _WIN32
    service = new Service(new Runner(), serviceMain, serviceControl);
#else
    service = new Service(new Runner());
#endif

    std::string functional;
    if(argc >=1)
        functional = argv[1];

    if( argc <= 1)
        service->start();
    else if(functional == "-i")
        service->install();
    else if(functional == "-u")
        service->uninstall();

    int exitcode = service->getExitCode();
    delete service;

    return exitcode;
}
