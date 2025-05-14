#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <iomanip>

using namespace std;

// Сортировка пузырьком
void bubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// Сортировка слиянием
void merge(vector<int>& arr, int left, int mid, int right) {
    vector<int> temp(arr.size());
    int i = left, j = mid + 1, k = left;
    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) {
            temp[k] = arr[i++];
        }
        else {
            temp[k] = arr[j++];
        }
        k++;
    }
    while (i <= mid) temp[k++] = arr[i++];
    while (j <= right) temp[k++] = arr[j++];
    for (int p = left; p <= right; p++) {
        arr[p] = temp[p];
    }
}

void mergeSort(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

// Быстрая сортировка
int partition(vector<int>& arr, int low, int high) {
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

void quickSort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// Генерация случайного массива
vector<int> generateRandomArray(int size) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, 10000);
    vector<int> arr(size);
    for (int i = 0; i < size; i++) {
        arr[i] = dis(gen);
    }
    return arr;
}

// Замеры времени
double measureTime(void (*sortFunc)(vector<int>&, int, int), vector<int>& arr) {
    auto start = chrono::high_resolution_clock::now();
    sortFunc(arr, 0, arr.size() - 1);
    auto end = chrono::high_resolution_clock::now();
    return chrono::duration<double, milli>(end - start).count();
}

double measureTimeBubble(vector<int>& arr) {
    auto start = chrono::high_resolution_clock::now();
    bubbleSort(arr);
    auto end = chrono::high_resolution_clock::now();
    return chrono::duration<double, milli>(end - start).count();
}

int main() {
    vector<int> sizes = { 1000, 5000, 10000, 50000 };
    cout << fixed << setprecision(3);
    cout << "Размер массива | Пузырьком (мс) | Слиянием (мс) | Быстрая (мс)\n";
    cout << "---------------|---------------|--------------|-------------\n";

    for (int size : sizes) {
        double bubbleTime = 0, mergeTime = 0, quickTime = 0;
        int runs = 10;

        for (int i = 0; i < runs; i++) {
            vector<int> arr = generateRandomArray(size);

            vector<int> arrCopy = arr;
            bubbleTime += measureTimeBubble(arrCopy);

            arrCopy = arr;
            mergeTime += measureTime(mergeSort, arrCopy);

            arrCopy = arr;
            quickTime += measureTime(quickSort, arrCopy);
        }

        bubbleTime /= runs;
        mergeTime /= runs;
        quickTime /= runs;

        cout << setw(14) << size << " | "
            << setw(13) << bubbleTime << " | "
            << setw(12) << mergeTime << " | "
            << setw(11) << quickTime << "\n";
    }
    return 0;
}