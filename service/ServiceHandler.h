#ifndef SERVICE_SERVICEHANDLER_H
#define SERVICE_SERVICEHANDLER_H

#include "Service.h"

std::shared_ptr<service::Service> server = nullptr;

#ifdef _WIN32
void WINAPI serviceMain(DWORD ac, LPTSTR* av) {
	server->service(ac, av);
}

void WINAPI serviceControl(DWORD opcode) {
	server->control(opcode);
}
#endif

namespace service {

	class ServiceHandler {
	private:
		std::shared_ptr<IServiceRunner> p_runner = nullptr;

	public:
		explicit ServiceHandler(std::shared_ptr<IServiceRunner> runner)
			: p_runner(std::move(runner)) {}

		int run(int argc, char *argv[]) {
#ifdef _WIN32
			server = std::make_shared<Service>(this->p_runner, serviceMain, serviceControl);
#else
			server = std::make_shared<Service>(this->p_runner);
#endif

			std::string functional;
			if (argc >= 2)
				functional = argv[1];

			if (argc <= 1)
				server->start();
			else if (functional == "install" || functional == "-i")
				server->install();
			else if (functional == "uninstall" || functional == "-u")
				server->uninstall();
			else if (functional == "no-service" || functional == "-n")
				return this->p_runner->Run();

			return server->getExitCode();
		}
	};

}

#endif //SERVICE_SERVICEHANDLER_H
