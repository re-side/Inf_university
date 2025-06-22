#include <iostream>
#include <vector>
#include <numeric>

// функция для печати вектора
void print_partition(const std::vector<int>& partition) {
    for (size_t i = 0; i < partition.size(); ++i) {
        std::cout << partition[i] << (i == partition.size() - 1 ? "" : " ");
    }
    std::cout << std::endl;
}

// Рекурсивная функция с возвратом
void find_partitions_recursive(int target, int start_num, std::vector<int>& current_partition) {
    if (target == 0) {
        print_partition(current_partition);
        return;
    }

    // перебираем возможные слагаемые
    for (int i = start_num; i <= target; ++i) {
        // Добавляем число
        current_partition.push_back(i);

        // Рекурсивно ищем разбиение для оставшейся суммы
        find_partitions_recursive(target - i, i, current_partition);

        // убираем последнее добавленное число, чтобы попробовать другие варианты
        current_partition.pop_back();
    }
}

// Основная функция
void generate_partitions(int n) {
    std::vector<int> current_partition;
    find_partitions_recursive(n, 1, current_partition);
}

int main() {
    int n = 4;
    std::cout << "Partitions for n = " << n << ":" << std::endl;
    generate_partitions(n);

    return 0;
}