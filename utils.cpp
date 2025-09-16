#include "utils.h"
#include <chrono>
#include <sstream>
#include <iomanip>

std::string getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    // std::localtime is not thread-safe, but here it's only called from the main thread.
    ss << std::put_time(std::localtime(&in_time_t), "%H:%M:%S");
    return ss.str();
}