#ifndef SERVICE_SERVICE_H
#define SERVICE_SERVICE_H

#include "IServiceRunner.h"

#ifndef _WIN32

#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>

#endif

namespace service {

	class Service {
	private:
		std::shared_ptr<IServiceRunner> p_runner;
		int p_err;

		bool p_isStarted;
		bool p_isPaused;
		char p_name[36];

#ifdef _WIN32
		LPSERVICE_MAIN_FUNCTION p_fpSrvMain;
		LPHANDLER_FUNCTION      p_fpSrvControl;

		SERVICE_TABLE_ENTRY     p_dispatchTable[2];
		SERVICE_STATUS          p_stat;
		SERVICE_STATUS_HANDLE   ph_stat;
#endif

	public:
#ifdef _WIN32
		Service(std::shared_ptr<IServiceRunner> runner, LPSERVICE_MAIN_FUNCTION funp_srvmain, LPHANDLER_FUNCTION funp_ctrl)
			: p_runner(runner), p_fpSrvMain(funp_srvmain), p_fpSrvControl(funp_ctrl), p_err(SERVICE_NO_ERROR)
		{
			//== copy the service name - if needed add UNICODE support here
			memset(this->p_name, 0, sizeof(this->p_name));
			strncpy_s(this->p_name, sizeof(this->p_name), this->p_runner->getName().c_str(), sizeof(this->p_name) - 1);

			//== clear the status flags
			this->p_isStarted = false;
			this->p_isPaused = false;

			//== clear the dispatch table
			memset(&this->p_dispatchTable[0], 0, sizeof(this->p_dispatchTable));

			//== clear the SERVICE_STATUS data structure
			memset(&this->p_stat, 0, sizeof(SERVICE_STATUS));
			this->ph_stat = 0;

			//== Now initialize SERVICE_STATUS here so we can
			//== call SetAcceptedControls() before Startup() or
			//== override in constructor of child class.
			this->p_stat.dwServiceType = SERVICE_WIN32;
			this->p_stat.dwCurrentState = SERVICE_START_PENDING;
			this->p_stat.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE | SERVICE_ACCEPT_SHUTDOWN;
		}

		int service(DWORD argc, LPTSTR* argv) {
			this->ph_stat = RegisterServiceCtrlHandler(TEXT(this->p_runner->getName().c_str()), this->p_fpSrvControl);
			if ((SERVICE_STATUS_HANDLE)0 == this->ph_stat) {
				this->p_err = static_cast<int>(::GetLastError());
				return this->p_err;
			}

			if (this->p_runner->Init(argc, argv) != NO_ERROR) {
				this->ChangeStatus(SERVICE_STOPPED);
				return this->p_err;
			}

			this->ChangeStatus(SERVICE_RUNNING);
			return this->p_runner->Run();
		}

		void control(DWORD opcode) {
			switch (opcode) {
			case SERVICE_CONTROL_PAUSE:
				ChangeStatus(SERVICE_PAUSE_PENDING);
				if (this->p_runner->OnPause() == NO_ERROR) {
					p_isPaused = true;
					ChangeStatus(SERVICE_PAUSED);
				}
				break;

			case SERVICE_CONTROL_CONTINUE:
				ChangeStatus(SERVICE_CONTINUE_PENDING);
				if (this->p_runner->OnContinue() == NO_ERROR) {
					p_isPaused = false;
					ChangeStatus(SERVICE_RUNNING);
				}
				break;

			case SERVICE_CONTROL_STOP:
				ChangeStatus(SERVICE_STOP_PENDING);
				this->p_runner->OnStop();
				ChangeStatus(SERVICE_STOPPED);
				break;

			case SERVICE_CONTROL_SHUTDOWN:
				ChangeStatus(SERVICE_STOP_PENDING);
				this->p_runner->OnShutdown();
				ChangeStatus(SERVICE_STOPPED);
				break;

			case SERVICE_CONTROL_INTERROGATE:
				this->p_runner->OnInquire();
				SetServiceStatus(this->ph_stat, &this->p_stat);
				break;

			default:
				this->p_runner->OnUserControl(opcode);
				SetServiceStatus(this->ph_stat, &this->p_stat);
				break;
			};
			return;
		}

		void ChangeStatus(DWORD state, DWORD checkpoint = (DWORD)0, DWORD waithint = (DWORD)0) {
			this->p_stat.dwCurrentState = state;
			this->p_stat.dwCheckPoint = checkpoint;
			this->p_stat.dwWaitHint = waithint;

			SetServiceStatus(this->ph_stat, &this->p_stat);
		}
#else
		Service(std::shared_ptr<IServiceRunner> runner) : p_runner(runner) {}
#endif

		~Service() {
			this->p_runner->OnShutdown();
		}

		/**
		 * install the service
		 *
		 * @return true - if installation worked / false - if not
		 */
		bool install() {
#ifdef _WIN32
			//== check to see if we are already installled
			if (this->isInstalled())
				return true;

			//== get the service manager handle
			SC_HANDLE schSCMgr = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
			if (schSCMgr == NULL) {
				this->p_err = static_cast<int>(::GetLastError());
				return false;
			}

			//== get the full path of the executable
			char szFilePath[MAX_PATH];
			::GetModuleFileName(NULL, szFilePath, sizeof(szFilePath));


			//== install the service
			SC_HANDLE schSrv = CreateService(
				schSCMgr,
				this->p_runner->getName().c_str(),
				this->p_runner->getName().c_str(),
				SERVICE_ALL_ACCESS,
				SERVICE_WIN32_OWN_PROCESS,
				SERVICE_DEMAND_START,
				SERVICE_ERROR_NORMAL,
				szFilePath,
				NULL,
				NULL,
				NULL,
				NULL,
				NULL
			);

			bool ret = true;
			if (schSrv == NULL) {
				this->p_err = static_cast<int>(::GetLastError());
				ret = false;
			}
			else
				this->p_runner->Install(); //== overload to add registry entries if needed

			//== All done, so pick up our toys and go home!
			CloseServiceHandle(schSrv);
			CloseServiceHandle(schSCMgr);

			return ret;
#else
			//TODO: Unix
			//check if init or systemd
			//install accordingly
			//example: http://orientdb.com/docs/2.1/Unix-Service.html
			return false;
#endif
		}

		/**
		 * uninstall the service
		 *
		 * @return true - if uninstallation worked / false - if not
		 */
		bool uninstall() {
#ifdef _WIN32
			if (!this->isInstalled())
				return true;

			//== get the service manager handle
			SC_HANDLE schSCMgr = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
			if (schSCMgr == NULL) {
				this->p_err = static_cast<int>(::GetLastError());
				return false;
			}

			//== Get the service handle
			SC_HANDLE schSrv = OpenService(schSCMgr, this->p_runner->getName().c_str(), DELETE);
			if (schSrv == NULL) {
				this->p_err = static_cast<int>(::GetLastError());
				CloseServiceHandle(schSCMgr);
				return false;
			}

			//== Now delete the service
			bool bRetval = true;
			if (!DeleteService(schSrv)) {
				bRetval = false;
				this->p_err = static_cast<int>(::GetLastError());
			}

			//== do any application specific cleanup
			this->p_runner->UnInstall();

			//== Put away toys and go home!
			CloseServiceHandle(schSrv);
			CloseServiceHandle(schSCMgr);

			return bRetval;
#else
			//TODO: Unix
			//check if init or systemd
			//uninstall accordingly
			//example: http://orientdb.com/docs/2.1/Unix-Service.html
			return false;
#endif
		}

		/**
		 * check if the service is installed
		 *
		 * @return true - if installed / false - if not
		 */
		bool isInstalled() {
#ifdef _WIN32
			bool bResult = false;

			// Open the Service Control Manager
			SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
			if (hSCM) {
				// Try to open the service
				SC_HANDLE hService = ::OpenService(hSCM, this->p_runner->getName().c_str(), SERVICE_QUERY_CONFIG);
				if (hService) {
					bResult = true;
					::CloseServiceHandle(hService);
				}
				::CloseServiceHandle(hSCM);
			}

			return bResult;
#else
			//TODO: Unix
			//check if init or systemd
			//example: http://orientdb.com/docs/2.1/Unix-Service.html
			//get if installed in the system
			return false;
#endif
		}

		/**
		 * start the service
		 *
		 * @return the return code from the finished service
		 */
		int start() {
			this->p_runner->getLog()->writeLine("starting service");

#ifdef _WIN32
			this->p_dispatchTable[0].lpServiceName = this->p_name;     //== Note: these
			this->p_dispatchTable[0].lpServiceProc = this->p_fpSrvMain; //   are pointers

			//== starts the service
			if (!StartServiceCtrlDispatcher(this->p_dispatchTable)) {
				this->p_err = static_cast<int>(::GetLastError());
				return this->p_err;
			}

			return SERVICE_NO_ERROR;
#else
			pid_t pid, sid;

			//Fork the Parent Process
			pid = fork();

			if (pid < 0) { exit(SERVICE_ERROR); }

			//We got a good pid, Close the Parent Process
			if (pid > 0) { exit(SERVICE_NO_ERROR); }

			//Change File Mask
			umask(0);

			//Create a new Signature Id for our child
			sid = setsid();
			if (sid < 0) { exit(SERVICE_ERROR); }

			//Change Directory
			//If we cant find the directory we exit with failure.
			if (chdir("/") < 0) { exit(SERVICE_ERROR); }

			//Close Standard File Descriptors
			close(STDIN_FILENO);
			close(STDOUT_FILENO);
			close(STDERR_FILENO);

			//----------------
			//Main Process with return value
			//----------------
			return this->p_runner->Run();
#endif
		}

		/**
		 * get the services exit code
		 *
		 * @return the exit code after the service finished
		 */
		int getExitCode() {
			if (this->p_err == SERVICE_NO_ERROR)
				return SERVICE_NO_ERROR;

#ifdef _WIN32
			return static_cast<int>(this->p_stat.dwWin32ExitCode);
#else
			return SERVICE_ERROR;
#endif
		}

		int getLastError() {
			return this->p_err;
		}
	};

}

#endif //SERVICE_SERVICE_H
