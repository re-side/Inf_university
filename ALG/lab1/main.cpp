#include <iostream>
#include <vector>
#include <chrono>

using namespace std;

void removeLastElement(vector<int>& arr) {
    if (!arr.empty()) {
        arr.pop_back();
    }
}

int main() {

    setlocale(LC_ALL, "Russian");

    vector<int> sizes = { 10, 100, 150, 1000, 10000, 10500, 100000, 1000000, 10000000, 100000000 };

    for (int size : sizes) {
        vector<int> arr(size, 1);

        auto start = chrono::high_resolution_clock::now();

        removeLastElement(arr);

        auto end = chrono::high_resolution_clock::now();

        chrono::duration<double> duration = end - start;
        cout << "Размер массива: " << size << ", Время выполнения: " << duration.count() << " секунд" << endl;
    }

    return 0;
}
