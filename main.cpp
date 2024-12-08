#include <iostream>
#include <set>
#include <vector>


bool isMultipleOf5(int num) {
    return num % 5 == 0;
}


int main() {
    int N;
    std::cout << "¬ведите количество элементов в множестве A: ";
    std::cin >> N;

    std::set<int> setA;
    std::cout << "¬ведите элементы множества A:" << std::endl;
    for (int i = 0; i < N; ++i) {
        int num;
        std::cin >> num;
        setA.insert(num);
    }

    std::cout << "¬ведите количество элементов в множестве B: ";
    std::cin >> N;

    std::set<int> setB;
    std::cout << "¬ведите элементы множества B:" << std::endl;
    for (int i = 0; i < N; ++i) {
        int num;
        std::cin >> num;
        setB.insert(num);
    }

    std::set<int> result;
    // ќбъедин€ем множества A и B
    for (int num : setA) {
        result.insert(num);
    }
    for (int num : setB) {
        result.insert(num);
    }

    // ‘ильтруем результат, оставл€€ только числа, кратные 5
    std::vector<int> finalResult;
    for (int num : result) {
        if (isMultipleOf5(num)) {
            finalResult.push_back(num);
        }
    }

    std::cout << "ћножество, состо€щее из элементов, кратных 5 и присутствующих хот€ бы в одном из множеств:" << std::endl;
    if (finalResult.empty()) {
        std::cout << "ћножество пустое" << std::endl;
    }
    else {
        for (int i = 0; i < finalResult.size(); ++i) {
            std::cout << finalResult[i] << (i == finalResult.size() - 1 ? "" : ", ");
        }
        std::cout << std::endl;
    }

    return 0;
}

