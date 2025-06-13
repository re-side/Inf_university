#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <omp.h>

int partition(std::vector<int>& arr, int low, int high) {
    int pivot = arr[high];
    int i = (low - 1);
    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);
    return (i + 1);
}

void quicksortSequential(std::vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quicksortSequential(arr, low, pi - 1);
        quicksortSequential(arr, pi + 1, high);
    }
}

void quicksortParallel_task(std::vector<int>& arr, int low, int high) {
    if (low < high) {
        if ((high - low) < 1000) {
            quicksortSequential(arr, low, high);
        }
        else {
            int pi = partition(arr, low, high);

#pragma omp task
            {
                quicksortParallel_task(arr, low, pi - 1);
            }

#pragma omp task
            {
                quicksortParallel_task(arr, pi + 1, high);
            }
        }
    }
}

void runQuicksortParallel(std::vector<int>& arr, int num_threads) {

#pragma omp parallel num_threads(num_threads)
    {
#pragma omp single
        {
            quicksortParallel_task(arr, 0, arr.size() - 1);
        }
    }
}

std::vector<int> generate_random_vector(int size) {
    std::vector<int> vec(size);
    for (int i = 0; i < size; ++i) {
        vec[i] = rand() % 10000;
    }
    return vec;
}

int main() {
    setlocale(LC_ALL, "Russian");

    std::vector<int> sizes = { 100, 1000, 10000, 20000, 30000, 40000, 50000 };
    std::vector<int> threads = { 2, 4, 8 };

    printf("%-15s %-15s %-15s %-15s %-15s\n", "Размер массива", "БС (сек)", "БС_П 2 потока", "БС_П 4 потока", "БС_П 8 потоков");

    for (int size : sizes) {
        printf("%-15d ", size);

        std::vector<int> arr_seq = generate_random_vector(size);
        auto start = std::chrono::high_resolution_clock::now();
        quicksortSequential(arr_seq, 0, arr_seq.size() - 1);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> seq_duration = end - start;
        printf("%-15.7f ", seq_duration.count());

        for (int num_threads : threads) {
            std::vector<int> arr_par = generate_random_vector(size);
            start = std::chrono::high_resolution_clock::now();
            runQuicksortParallel(arr_par, num_threads);
            end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> par_duration = end - start;
            printf("%-15.7f ", par_duration.count());
        }
        printf("\n");
    }

    return 0;
}