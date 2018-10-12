#ifndef SERVICE_ILOG_H
#define SERVICE_ILOG_H

#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
#include "Semaphore.h"

#define SERVICE_NO_ERROR    0
#define SERVICE_ERROR       1

namespace service {

	class ILog {
	private:
		Semaphore p_semaphore;

	protected:
		virtual void writeOut(const std::string &text) {
			std::cout << text;
		}

	public:
		/**
		 * get system style new line string
		 */
		std::string getNewLine() {
#ifdef _WIN32
			return "\r\n";
#else
			return "\n";
#endif
		}

		std::string getCurrentTimeAndDate() {
			return this->getCurrentTime("%Y-%m-%d %H:%M:%S");
		}

		std::string getCurrentDate() {
			return this->getCurrentTime("%Y-%m-%d");
		}

		std::string getCurrentTime(const std::string &mod) {
			auto now = std::chrono::system_clock::now();
			auto in_time_t = std::chrono::system_clock::to_time_t(now);

			/*std::stringstream ss;
			ss << std::put_time(std::localtime(&in_time_t), mod.c_str());
			return ss.str();*/

			char foo[24];
			struct tm timeinfo;

#ifdef _WIN32
			localtime_s(&timeinfo, &in_time_t);
#else
			timeinfo = *localtime(&in_time_t);
#endif

			if (0 < strftime(foo, sizeof(foo), mod.c_str(), &timeinfo))
				return std::string(foo);
			else
				return "";
		}

		void writeLine(const std::string &text) {
			this->write(text + this->getNewLine());
		}

		void write(const std::string &text) {
			this->p_semaphore.acquire();
			this->writeOut(this->getCurrentTimeAndDate() + " || " + text);
			this->p_semaphore.release();
		}

		void writeError(const std::string &className, const std::string &functionName, const std::string &errorText) {
			this->writeLine(className + " :: " + functionName + " :: " + errorText);
		}

		void writeError(const std::string &className, const std::string &functionName, int errorCode) {
			this->writeError(className, functionName, std::to_string(errorCode));
		}

		void writeError(const std::string &className, const std::string &functionName, std::exception &e) {
			this->writeError(className, functionName, e.what());
		}
	};

}

#endif //SERVICE_ILOG_H
