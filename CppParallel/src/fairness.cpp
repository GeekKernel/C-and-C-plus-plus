/**
 * @file fairness.cpp
 * @author Wei Li (Ithaca) (weili_yzzcq@163.com)
 * @brief A simple using using a spinlock under heavy contention
 * @version 0.1
 * @date 2025-02-24
 * 
 * @copyright Copyright (c) 2025
 * 
 * g++ src/fairness.cpp -o bin/Release/fairness.exe -std=c++20
 * clang++ src/fairness.cpp -o bin/Release/fairness.exe -std=c++20
 * 
 */

#include <emmintrin.h>

#include <array>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <thread>
#include <vector>

// Ticket Spinlock
class Spinlock
{
private:
    // Lock is now two counters:
    //  1.) The latest place taken in line
    //  2.) Which number is currently being served
    std::atomic<std::uint16_t> line{0};
    volatile std::uint16_t     serving{0};

public:
    void lock()
    {
        // Get the latest place in line (and increment the value)
        auto place = line.fetch_add(1);

        // Wait until our number is "called"
        while (serving != place) _mm_pause();
    }

    // Unlocking mechanism
    // Increment serving number to pass the lock
    // No need for an atomic! The thread with the lock is the only one that
    // accesses this variable!
    void unlock()
    {
        asm volatile("" : : : "memory"); // GCC and Clang compiler
        serving = serving + 1;
    }
};

// -------------------------------------
int main(int argc, const char *argv[])
{
    // Number of iterations
    const int num_iterations = 1 << 22;

    // Number of threads and iterations per thread
    const int num_threads = 8;

    // Keep track of per-thread average wait time
    std::array<int, 8> max_wait_time;

    // Our lock guarding access to the list
    Spinlock spinlock;

    // Function that removes items from the list in parallel
    auto work = [&](int tid)
    {
        int max = 0;
        for (int i = 0; i < num_iterations; i++)
        {
            // Time how long it takes to get the lock
            auto start = std::chrono::system_clock::now();
            spinlock.lock();
            auto stop = std::chrono::system_clock::now();
            spinlock.unlock();

            // Calculate how long in took to get the lock
            int diff = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();

            // Save the current max
            max = std::max(max, diff);
        }
        // Save the final max value
        max_wait_time[tid] = max;
    };

    // Spawn threads
    std::vector<std::thread> threads;
    for (int i = 0; i < num_threads; i++)
    {
        threads.emplace_back(work, i);
    }

    // Join threads
    for (auto &thread : threads)
    {
        thread.join();
    }

    // Print out the max wait times
    for (auto max : max_wait_time)
    {
        std::cout << max << ' ';
    }
    std::cout << '\n';

    return 0;
}
