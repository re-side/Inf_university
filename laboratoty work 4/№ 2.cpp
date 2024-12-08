#include <iostream>
#include <set>
#include <vector>


bool isMultipleOf5(int num) {
    return num % 5 == 0;
}


int main() {
    int N;
    std::cout << "������� ���������� ��������� � ��������� A: ";
    std::cin >> N;

    std::set<int> setA;
    std::cout << "������� �������� ��������� A:" << std::endl;
    for (int i = 0; i < N; ++i) {
        int num;
        std::cin >> num;
        setA.insert(num);
    }

    std::cout << "������� ���������� ��������� � ��������� B: ";
    std::cin >> N;

    std::set<int> setB;
    std::cout << "������� �������� ��������� B:" << std::endl;
    for (int i = 0; i < N; ++i) {
        int num;
        std::cin >> num;
        setB.insert(num);
    }

    std::set<int> result;
    // ���������� ��������� A � B
    for (int num : setA) {
        result.insert(num);
    }
    for (int num : setB) {
        result.insert(num);
    }

    // ��������� ���������, �������� ������ �����, ������� 5
    std::vector<int> finalResult;
    for (int num : result) {
        if (isMultipleOf5(num)) {
            finalResult.push_back(num);
        }
    }

    std::cout << "���������, ��������� �� ���������, ������� 5 � �������������� ���� �� � ����� �� ��������:" << std::endl;
    if (finalResult.empty()) {
        std::cout << "��������� ������" << std::endl;
    }
    else {
        for (int i = 0; i < finalResult.size(); ++i) {
            std::cout << finalResult[i] << (i == finalResult.size() - 1 ? "" : ", ");
        }
        std::cout << std::endl;
    }

    return 0;
}

