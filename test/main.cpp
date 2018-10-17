#include "ServiceHandler.h"
#include "Log.h"

class Runner : public service::IServiceRunner {
private:
    bool p_running;

public:
    explicit Runner()
            : IServiceRunner(std::make_shared<Log>()), p_running(false) {}

    std::string getName() override {
        return "MyService";
    }

    int Run() override {
        this->p_running = true;
        int i = 0;

        while (this->p_running) {
            std::this_thread::sleep_for(std::chrono::seconds(3));
            this->getLog()->writeError("Runner", "Run", i++);
        }

        this->getLog()->writeLine("Shutdown called");
        return EXIT_SUCCESS;
    }

    void OnStop() override {
        this->p_running = false;
    }
};

int main(int argc, char *argv[]) {
    std::shared_ptr<service::ServiceHandler> handler = std::make_shared<service::ServiceHandler>(std::make_shared<Runner>());
    return handler->run(argc, argv);
}
