#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <omp.h>
#include <random>

void swap(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

int partition(std::vector<int>& arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quicksortSequential(std::vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quicksortSequential(arr, low, pi - 1);
        quicksortSequential(arr, pi + 1, high);
    }
}

void quicksortParallel(std::vector<int>& arr, int low, int high, int num_threads, int depth = 0) {
    if (low < high) {
        if (high - low < 1000 || depth > 4) {
            quicksortSequential(arr, low, high);
        }
        else {
            int pi = partition(arr, low, high);
#pragma omp parallel sections num_threads(num_threads)
            {
#pragma omp section
                {
                    quicksortParallel(arr, low, pi - 1, num_threads, depth + 1);
                }
#pragma omp section
                {
                    quicksortParallel(arr, pi + 1, high, num_threads, depth + 1);
                }
            }
        }
    }
}

std::vector<int> generateRandomArray(int size) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100000);
    std::vector<int> arr(size);
    for (int i = 0; i < size; i++) {
        arr[i] = dis(gen);
    }
    return arr;
}

double measureTime(std::vector<int>& arr, void (*sortFunc)(std::vector<int>&, int, int)) {
    auto start = std::chrono::high_resolution_clock::now();
    sortFunc(arr, 0, arr.size() - 1);
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double>(end - start).count();
}

double measureTimeParallel(std::vector<int>& arr, int num_threads) {
    auto start = std::chrono::high_resolution_clock::now();
    quicksortParallel(arr, 0, arr.size() - 1, num_threads);
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double>(end - start).count();
}

int main() {
    setlocale(LC_ALL, "Russian");
    std::vector<int> sizes = { 100, 1000, 10000, 20000, 30000, 40000, 50000 };
    std::vector<int> threads = { 2, 4, 8 };

    std::cout << "Размер массива\tБС (сек)\tБС_П 2 потока (сек)\tБС_П 4 потока (сек)\tБС_П 8 потока (сек)\n";
    std::vector<std::vector<double>> times(sizes.size(), std::vector<double>(4, 0.0));

    for (size_t i = 0; i < sizes.size(); i++) {
        int size = sizes[i];
        std::vector<int> arr = generateRandomArray(size);

        std::vector<int> arr_seq = arr;
        times[i][0] = measureTime(arr_seq, quicksortSequential);

        for (size_t j = 0; j < threads.size(); j++) {
            std::vector<int> arr_par = arr;
            omp_set_num_threads(threads[j]);
            times[i][j + 1] = measureTimeParallel(arr_par, threads[j]);
        }

        std::cout << size << "\t" << times[i][0] << "\t" << times[i][1] << "\t" << times[i][2] << "\t" << times[i][3] << "\n";
    }

    std::cout << "\nТаблица коэффициентов ускорения:\n";
    std::cout << "Размер массива\tВремя БС (с)\tВремя 2 потока (с)\tВремя 4 потока (с)\tВремя 8 потока (с)\tSpeedup (2)\tSpeedup (4)\tSpeedup (8)\n";
    for (size_t i = 0; i < sizes.size(); i++) {
        double seq_time = times[i][0];
        std::cout << sizes[i] << "\t" << seq_time;
        for (size_t j = 0; j < threads.size(); j++) {
            std::cout << "\t" << times[i][j + 1];
        }
        for (size_t j = 0; j < threads.size(); j++) {
            double speedup = seq_time / times[i][j + 1];
            std::cout << "\t" << speedup;
        }
        std::cout << "\n";
    }

    return 0;
}