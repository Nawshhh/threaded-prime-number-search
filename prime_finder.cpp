#include "prime_finder.h"
#include "utils.h" // getCurrentTimestamp
#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>
#include <cmath>

void PrimeFinder::run(const Config& config) {
    found_primes.clear();
    std::vector<std::thread> workers;

    if (config.division_mode == "range") {
        int range_size = config.limit / config.threads;
        for (int i = 0; i < config.threads; ++i) {
            int start = i * range_size + 1;
            int end = (i == config.threads - 1) ? config.limit : (i + 1) * range_size;
            workers.emplace_back(&PrimeFinder::workerRange, this, i, start, end, config.print_mode);
        }
    } else if (config.division_mode == "linear") {
        runDivisibility(config);
    }

    for (auto& worker : workers) {
        worker.join();
    }

    if (config.print_mode == "wait") {
        std::sort(found_primes.begin(), found_primes.end());
    }
}
const std::vector<PrimeResult>& PrimeFinder::getPrimes() const {
    return found_primes;
}
bool PrimeFinder::isPrime(int n) const {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (int i = 5; i * i <= n; i = i + 6) {
        if (n % i == 0 || n % (i + 2) == 0)
            return false;
    }
    return true;
}

void PrimeFinder::workerRange(int thread_id, int start, int end, const std::string& print_mode) {
    std::vector<PrimeResult> local_results;
    for (int i = start; i <= end; ++i) {
        if (isPrime(i)) {
            if (print_mode == "immediate") {
                std::lock_guard<std::mutex> lock(io_mutex);
                std::cout << "[" << getCurrentTimestamp() << "] Thread " << thread_id 
                          << " (" << start << "-" << end << ") found a prime number: " << i << std::endl;
            } else { 
                local_results.push_back({i, thread_id, getCurrentTimestamp(), start, end});
            }
        }
    }
    
    if (print_mode == "wait" && !local_results.empty()) {
        std::lock_guard<std::mutex> lock(vector_mutex);
        found_primes.insert(found_primes.end(), local_results.begin(), local_results.end());
    }
}

void PrimeFinder::workerLinear(int thread_id, int start_num, int step, int limit, const std::string& print_mode) {
    std::vector<PrimeResult> local_results;
    for (int i = start_num; i <= limit; i += step) {
        if (isPrime(i)) {
            if (print_mode == "immediate") {
                std::lock_guard<std::mutex> lock(io_mutex);
                std::cout << "[" << getCurrentTimestamp() << "] Thread " << thread_id 
                          << " found a prime number: " << i << std::endl;
            } else { 
                local_results.push_back({i, thread_id, getCurrentTimestamp()});
            }
        }
    }

    if (print_mode == "wait" && !local_results.empty()) {
        std::lock_guard<std::mutex> lock(vector_mutex);
        found_primes.insert(found_primes.end(), local_results.begin(), local_results.end());
    }
}

bool PrimeFinder::checkDivisibilitySegment(int n, int start, int end) const {
    if (n % 2 == 0 && n != 2) return false;
    for (int d = start; d <= end; ++d) {
        if (n % d == 0) return false;
    }
    return true;
}

void PrimeFinder::runDivisibility(const Config& config) {
    found_primes.clear();

    for (int n = 2; n <= config.limit; ++n) {
        if (n <= 3) { // base cases
            recordPrime(n, 0, config.print_mode);
            continue;
        }

        int sqrtN = static_cast<int>(std::sqrt(n));
        int chunk = (sqrtN - 2) / config.threads + 1;

        std::vector<std::thread> workers;
        std::vector<bool> results(config.threads, true);

        for (int t = 0; t < config.threads; ++t) {
            int start = 2 + t * chunk;
            int end   = std::min(sqrtN, start + chunk - 1);

            workers.emplace_back([&, t, start, end]() {
                if (start <= end) {
                    std::lock_guard<std::mutex> lock(io_mutex);
                    std::cout << "[" << getCurrentTimestamp() << "] "
                            << "Thread " << t << " checking divisors "
                            << start << "-" << end << " for n=" << n
                            << std::endl;
                }
                results[t] = checkDivisibilitySegment(n, start, end);
            });
        }

        for (auto& w : workers) w.join();

        bool isPrime = std::all_of(results.begin(), results.end(),
                                   [](bool r) { return r; });

        if (isPrime) {
            recordPrime(n, 0, config.print_mode);
        }
    }

    if (config.print_mode == "wait") {
        std::sort(found_primes.begin(), found_primes.end());
    }
}

void PrimeFinder::recordPrime(int n, int thread_id, const std::string& print_mode) {
    if (print_mode == "immediate") {
        std::lock_guard<std::mutex> lock(io_mutex);
        std::cout << "[" << getCurrentTimestamp() << "] Thread " << thread_id
                  << " found a prime number: " << n << std::endl;
    } else {
        std::lock_guard<std::mutex> lock(vector_mutex);
        found_primes.push_back({n, thread_id, getCurrentTimestamp()});
    }
}