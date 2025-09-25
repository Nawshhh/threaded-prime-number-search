# Threaded C++ Prime Number Finder

A simple multi-threaded C++ program that finds prime numbers up to a specified limit. Its behavior is controlled through a `config.txt` file.

* **`[x] threads`**: The number of threads to use (must be > 0).
* **`[y] limit`**: The maximum number to check for primality (must be > 1).
* **`print_mode`**:
    * `immediate`: Prints primes instantly as they are found.
    * `wait`: Prints all primes in a sorted list at the end.
* **`division_mode`**:
    * `range`: Each thread gets a contiguous block of numbers.
    * `linear`: Threads take turns checking individual numbers.

---

## How to Build and Run

### 1. Requirements
* A C++ compiler that supports C++11 (like g++).

### 2. Compile
* Open your terminal in the project directory and run:
```
g++ *.cpp -o main -std=c++20
./main
```
