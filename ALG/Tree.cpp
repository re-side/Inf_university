#include <iostream>

// ��������� ���� ������
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// ����������� ��������������� �������
int find_paths_sum(TreeNode* node, int current_sum) {
    if (node == nullptr) {
        return 0; // ���� ��� �� ����������
    }

    // "�����������" ����� ����� � �������� �����
    int new_sum = current_sum * 10 + node->val;

    if (node->left == nullptr && node->right == nullptr) {
        return new_sum;
    }

    // ���������� ����������� ����� ��� ������ � ������� ���������
    return find_paths_sum(node->left, new_sum) + find_paths_sum(node->right, new_sum);
}

// �������� �������
int solution(TreeNode* root) {
    if (root == nullptr) {
        return 0;
    }
    return find_paths_sum(root, 0);
}

// ������ �������������
int main() {
    // ������ 1 �� �������: 1 -> 3, 1 -> 5
    TreeNode* root1 = new TreeNode(1);
    root1->left = new TreeNode(3);
    root1->right = new TreeNode(5);
    std::cout << "Test case 1: " << solution(root1) << std::endl; 

    // ������ 2 �� �������
    TreeNode* root2 = new TreeNode(1);
    root2->left = new TreeNode(2);
    root2->right = new TreeNode(3);
    root2->right->left = new TreeNode(2);
    root2->right->right = new TreeNode(1);
    std::cout << "Test case 2: " << solution(root2) << std::endl;

    return 0;
}