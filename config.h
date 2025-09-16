#ifndef CONFIG_H
#define CONFIG_H

#include <string>

struct Config {
    int threads;
    int limit;
    std::string print_mode;
    std::string division_mode;
};

bool readConfig(const std::string& filename, Config& out_config);

#endif 