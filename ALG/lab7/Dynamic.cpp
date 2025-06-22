#include <iostream>
#include <vector>
#include <numeric>

bool can_partition(const std::vector<int>& nums) {
    // ������� ����� ����� ���� �����
    int total_sum = std::accumulate(nums.begin(), nums.end(), 0);

    // ���� ����� ��������, ��������� �� �� ��� ������ ����� ����������
    if (total_sum % 2 != 0) {
        return false;
    }

    // ������� ����� ��� ������ �� ���� ������
    int target = total_sum / 2;

    // ���������� ������������ ���������������� ��� ������ ������������
    std::vector<bool> dp(target + 1, false);
    dp[0] = true; 

    // �������� �� ������� ����� �� �������� �������
    for (int num : nums) {
        for (int j = target; j >= num; --j) {
            if (dp[j - num]) {
                dp[j] = true;
            }
        }
    }

    // ���������
    return dp[target];
}

int main() {
    int n;

    // ������ 1
    n = 4;
    std::vector<int> nums1 = { 1, 5, 7, 1 }; 
    std::cout << "Test case 1: " << (can_partition(nums1) ? "True" : "False") << std::endl;

    // ������ 2
    n = 3;
    std::vector<int> nums2 = { 2, 10, 9 }; 
    std::cout << "Test case 2: " << (can_partition(nums2) ? "True" : "False") << std::endl;

    return 0;
}