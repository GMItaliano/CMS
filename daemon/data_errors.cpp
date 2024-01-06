#include "data_errors.h"


std::string logPath = "/var/log/error_daemon.log";

void logError(std::string msg){

    std::ofstream logfile(logPath, std::ios::app);
    if(logfile.is_open()){

        // Get current time
        std::time_t currentTime = std::time(nullptr);
        struct tm *localTime = std::localtime(&currentTime);

        logfile << "[" << std::asctime(localTime) << "] :: " << msg << std::endl;

        logfile.close();

    } else 
        std::cerr << "[ERROR] Unable to open LOG FILE" << std::endl;

}