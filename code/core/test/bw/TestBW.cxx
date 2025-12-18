#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <cmath>
#include <string>

typedef double DataType;

void work_func(DataType* data, size_t start, size_t end, bool compute_heavy) {
    if (!compute_heavy) {
        for (size_t i = start; i < end; ++i) {
            data[i] = data[i] * 0.5 + 0.2;
        }
    } else {
        for (size_t i = start; i < end; ++i) {
            DataType val = data[i];
            for (int j = 0; j < 100; ++j) {
                val = std::sqrt(std::sin(val) + 2.0);
            }
            data[i] = val;
        }
    }
}

int main(int argc, char** argv) {
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " <threads> <mode> <elements>" << std::endl;
        return 1;
    }

    int num_threads = std::stoi(argv[1]);
    std::string mode = argv[2];
    size_t N = std::stoull(argv[3]);
    bool compute_heavy = (mode == "compute");

    std::vector<DataType> data(N);
    std::vector<std::thread> threads;
    size_t chunk_size = N / num_threads;

    // --- PHASE 1: FIRST TOUCH INITIALIZATION ---
    for (int i = 0; i < num_threads; ++i) {
        size_t start = i * chunk_size;
        size_t end = (i == num_threads - 1) ? N : (i + 1) * chunk_size;
        threads.emplace_back([&, start, end]() {
            for (size_t j = start; j < end; ++j) data[j] = 1.0;
        });
    }
    for (auto& t : threads) t.join();
    threads.clear();

    // --- PHASE 2: BENCHMARK ---
    auto start_time = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < num_threads; ++i) {
        size_t start = i * chunk_size;
        size_t end = (i == num_threads - 1) ? N : (i + 1) * chunk_size;
        threads.emplace_back(work_func, data.data(), start, end, compute_heavy);
    }
    for (auto& t : threads) t.join();
    auto end_time = std::chrono::high_resolution_clock::now();

    std::cout << std::chrono::duration<double>(end_time - start_time).count() << std::endl;
    return 0;
}