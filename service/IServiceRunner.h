#ifndef SERVICE_ISERVICERUNNER_H
#define SERVICE_ISERVICERUNNER_H

#include "ILog.h"
#include <memory>

#ifdef _WIN32
#include <Windows.h>
#endif

namespace service {

	class IServiceRunner {
	private:
		std::shared_ptr<ILog> p_log = nullptr;

	public:
		explicit IServiceRunner(std::shared_ptr<ILog> log) : p_log(log) {}

		virtual std::string getName() = 0;

		virtual int Run() = 0;

		virtual int OnPause() {
			return SERVICE_NO_ERROR;
		}

		virtual int OnContinue() {
			return SERVICE_NO_ERROR;
		}

		virtual void OnStop() {}

		virtual void OnShutdown() {
			this->OnStop();
		}

		virtual void OnInquire() {}

#ifdef _WIN32
		virtual void OnUserControl(DWORD control) {
			this->getLog()->writeLine("OnUserControl: " + std::to_string(control));
		}

		virtual int Init(DWORD argc, LPTSTR* argv) {
			this->getLog()->writeLine("Init Arguments: ");
			for (DWORD i = 0; i < argc; i++)
				this->getLog()->writeLine("i || " + std::string(argv[i]));

			return SERVICE_NO_ERROR;
		}
#endif

		virtual void Install() {}

		virtual void UnInstall() {}

		std::shared_ptr<ILog> getLog() {
			return this->p_log;
		}
	};

}

#endif //SERVICE_ISERVICERUNNER_H
