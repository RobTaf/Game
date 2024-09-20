#ifndef Logger_hpp
#define Logger_hpp

#include <fstream>
#include <chrono>
#include <iomanip>
#include <ctime>

#include "Utility.hpp"

class Logger {
public:
    static void log(const std::string& message) {
        std::ofstream logFile;
        logFile.open(Utility::workingDirectory() + "log.dat", std::ios::app);
        if (not logFile.is_open()) {
            std::cerr << "! Failed opening log file" << std::endl;
        } else {
            logFile << message << " [ " << getCurrentTime() << " ] " << std::endl;
        }
        logFile.close();
    }

private:
    static std::string getCurrentTime() {
        auto now = std::chrono::system_clock::now();
        std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

        std::ostringstream oss;
        oss << std::put_time(std::localtime(&currentTime), "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
};

#endif