#include <iostream>
#include <vector>
#include <chrono>
#ifdef _WIN32
#include <windows.h>
#endif
using namespace std;

// Функция для обмена элементов
void swap(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

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
    int n1 = mid - left + 1;
    int n2 = right - mid;
    vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] < R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
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

int main() {
    // Настройка кодировки для Windows
#ifdef _WIN32
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
#endif

    // Тестовый массив
    vector<int> arr = { 64, 34, 25, 12, 22, 11, 90 };
    cout << "Исходный массив: ";
    for (int x : arr) cout << x << " ";
    cout << endl;

    // Сортировка пузырьком
    vector<int> arr_bubble = arr;
    auto start_bubble = chrono::high_resolution_clock::now();
    bubbleSort(arr_bubble);
    auto end_bubble = chrono::high_resolution_clock::now();
    auto bubble_time = chrono::duration_cast<chrono::nanoseconds>(end_bubble - start_bubble).count();
    cout << "Сортировка пузырьком: ";
    for (int x : arr_bubble) cout << x << " ";
    cout << "(Время: " << bubble_time << " нс)" << endl;

    // Сортировка слиянием
    vector<int> arr_merge = arr;
    auto start_merge = chrono::high_resolution_clock::now();
    mergeSort(arr_merge, 0, arr_merge.size() - 1);
    auto end_merge = chrono::high_resolution_clock::now();
    auto merge_time = chrono::duration_cast<chrono::nanoseconds>(end_merge - start_merge).count();
    cout << "Сортировка слиянием: ";
    for (int x : arr_merge) cout << x << " ";
    cout << "(Время: " << merge_time << " нс)" << endl;

    // Быстрая сортировка
    vector<int> arr_quick = arr;
    auto start_quick = chrono::high_resolution_clock::now();
    quickSort(arr_quick, 0, arr_quick.size() - 1);
    auto end_quick = chrono::high_resolution_clock::now();
    auto quick_time = chrono::duration_cast<chrono::nanoseconds>(end_quick - start_quick).count();
    cout << "Быстрая сортировка: ";
    for (int x : arr_quick) cout << x << " ";
    cout << "(Время: " << quick_time << " нс)" << endl;

    return 0;
}