#ifndef PRIME_FINDER_H
#define PRIME_FINDER_H

#include "config.h"
#include <vector>
#include <mutex>
#include <string>

struct PrimeResult {
    int number;
    int thread_id;
    std::string timestamp;
    int range_start = 0; 
    int range_end = 0; 

    // comparison operator to allow sorting the vector by the prime number
    bool operator<(const PrimeResult& other) const {
        return number < other.number;
    }
};

class PrimeFinder {
public:
    PrimeFinder() = default;

    void run(const Config& config);

    const std::vector<PrimeResult>& getPrimes() const;

private:
    void workerRange(int thread_id, int start, int end, const std::string& print_mode);
    void workerInterleaved(int thread_id, int start_num, int step, int limit, const std::string& print_mode);
    
    bool isPrime(int n) const;

    std::vector<PrimeResult> found_primes;
    std::mutex io_mutex;
    std::mutex vector_mutex;
};

#endif 