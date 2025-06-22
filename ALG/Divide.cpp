#include <iostream>
#include <vector>

int find_peak_element(const std::vector<int>& nums) {
    int left = 0;
    int right = nums.size() - 1;

    while (left < right) {
        int mid = left + (right - left) / 2;

        // ���������� ������� ������� � ��� ������ �������
        if (nums[mid] < nums[mid + 1]) {
            left = mid + 1;
        }
        else {
            right = mid;
        }
    }

    // left == right, �� ����� ���
    return nums[left];
}

int main() {
    // ������ �� �������
    std::vector<int> nums = { 1, 3, 20, 4, 1, 0 };
    std::cout << "Test case 1: " << find_peak_element(nums) << std::endl;

    return 0;
}