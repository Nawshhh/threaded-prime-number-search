#include <iostream>
#include "config.h"
#include "utils.h"
#include "prime_finder.h"

int main() {
    Config config;
    if (!readConfig("config.txt", config)) {
        std::cerr << "Terminating due to configuration error." << std::endl;
        return 1;
    }

    std::cout << "--- Configuration ---" << std::endl;
    std::cout << "Threads: " << config.threads << std::endl;
    std::cout << "Search up to: " << config.limit << std::endl;
    std::cout << "Print mode: " << config.print_mode << std::endl;
    std::cout << "Division mode: " << config.division_mode << std::endl;
    std::cout << "---------------------\n" << std::endl;
    
    std::cout << "Program start timestamp: " << getCurrentTimestamp() << "\n" <<std::endl;

    PrimeFinder finder;
    finder.run(config);

    if (config.print_mode == "wait") {
        const auto& results = finder.getPrimes();
        std::cout << "\n--- All threads finished. Printing all " << results.size() << " found primes. ---" << std::endl;
        
        for (const auto& result : results) {
            std::cout << "[" << result.timestamp << "] Thread " << result.thread_id;
            
            if (result.range_end > 0) {
                std::cout << " (" << result.range_start << "-" << result.range_end << ")";
            }
            
            std::cout << " found a prime number: " << result.number << std::endl;
        }
    }

    std::cout << "\nProgram end timestamp: " << getCurrentTimestamp() << std::endl;

    return 0;
}