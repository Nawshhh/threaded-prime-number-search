#include "utils.h"
#include <chrono>
#include <sstream>
#include <iomanip>

std::string getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%H:%M:%S"); // YYYY-MM-DD HH:MM:SS
    ss << '.' << std::setw(3) << std::setfill('0') << ms.count();       // .ms

    return ss.str();
}