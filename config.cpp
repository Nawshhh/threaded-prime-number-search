#include "config.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <charconv>   // from_chars
#include <cstdint>    // uint64_t

bool readConfig(const std::string& filename, Config& out_config) {
    std::ifstream config_file(filename);
    if (!config_file.is_open()) {
        std::cerr << "Error: Could not open config file '" << filename << "'." << std::endl;
        return false;
    }

    std::map<std::string, std::string> values;
    std::string line;
    while (std::getline(config_file, line)) {
        if (line.empty() || line[0] == '#') continue;
        
        std::istringstream iss(line);
        std::string key, value;
        if (std::getline(iss, key, ':') && std::getline(iss, value)) {
            key.erase(key.find_last_not_of(" \t\n\r") + 1);
            value.erase(0, value.find_first_not_of(" \t\n\r"));
            value.erase(value.find_last_not_of(" \t\n\r") + 1);
            values[key] = value;
        }
    }

    std::vector<std::string> required_keys = {"threads", "limit", "print_mode", "division_mode"};
    for (const auto& key : required_keys) {
        if (values.find(key) == values.end()) {
            std::cerr << "Error: Missing required key '" << key << "' in config file." << std::endl;
            return false;
        }
    }

    try {
        if (values["threads"].empty()) throw std::invalid_argument("is empty");
        int threads = std::stoi(values["threads"]);
        if (threads <= 0) throw std::out_of_range("must be greater than 0");
        out_config.threads = threads;
    } catch (const std::exception& e) {
        std::cerr << "Error: Invalid value for 'threads'. " << e.what() << "." << std::endl;
        return false;
    }

    try {
        if (values["limit"].empty())
            throw std::invalid_argument("is empty");

        uint64_t limit = 0;
        std::string s = values["limit"];

        auto [ptr, ec] = std::from_chars(s.data(), s.data() + s.size(), limit);
        if (ec == std::errc::invalid_argument)
            throw std::invalid_argument("not a number");
        if (ec == std::errc::result_out_of_range)
            throw std::out_of_range("must not exceed UINT64_MAX (18446744073709551615)");

        if (limit <= 1)
            throw std::out_of_range("must be greater than 1");

        out_config.limit = limit;
    } catch (const std::exception& e) {
        std::cerr << "Error: Invalid value for 'limit'. " << e.what() << "." << std::endl;
        return false;
    }

    const std::string& print_mode = values["print_mode"];
    if (print_mode != "immediate" && print_mode != "wait") {
        std::cerr << "Error: Invalid value for 'print_mode'. Must be 'immediate' or 'wait'." << std::endl;
        return false;
    }
    out_config.print_mode = print_mode;


    const std::string& division_mode = values["division_mode"];
    if (division_mode != "range" && division_mode != "linear") {
        std::cerr << "Error: Invalid value for 'division_mode'. Must be 'range' or 'linear'." << std::endl;
        return false;
    }
    out_config.division_mode = division_mode;

    return true; 
}