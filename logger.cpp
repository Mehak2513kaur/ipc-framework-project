#include "logger.h"
#include <fstream>

void logEvent(const std::string &msg) {
    std::ofstream file("logs/activity.log", std::ios::app);
    file << msg << "\n";
}
