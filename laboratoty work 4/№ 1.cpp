#include <iostream>


int countPositive(const int* arr, int size) {
    int count = 0;
    for (int i = 0; i < size; i++) {
        if (arr[i] > 0) {
            count++;
        }
    }
    return count;
}

void findArrayWithMorePositives(const int* arrA, const int* arrB, int size) {
    int posCountA = countPositive(arrA, size);
    int posCountB = countPositive(arrB, size);

    if (posCountA >= posCountB) {
        std::cout << "Массив A содержит больше или равно положительных элементов (" << posCountA << "):" << std::endl;
        for (int i = 0; i < size; i++) {
            std::cout << arrA[i] << (i == size - 1 ? "" : " ");
        }
        std::cout << std::endl;
    }
    else {
        std::cout << "Массив B содержит больше положительных элементов (" << posCountB << "):" << std::endl;
        for (int i = 0; i < size; i++) {
            std::cout << arrB[i] << (i == size - 1 ? "" : " ");
        }
        std::cout << std::endl;
    }
}

int main() {
    int N;
    std::cout << "Введите размер массивов (N): ";
    std::cin >> N;

    int* A = new int[N];
    int* B = new int[N];

    std::cout << "Введите " << N << " элементов для массива A:" << std::endl;
    for (int i = 0; i < N; i++) {
        std::cin >> A[i];
    }

    std::cout << "Введите " << N << " элементов для массива B:" << std::endl;
    for (int i = 0; i < N; i++) {
        std::cin >> B[i];
    }

    findArrayWithMorePositives(A, B, N);

    delete[] A;
    delete[] B;

    return 0;
}

