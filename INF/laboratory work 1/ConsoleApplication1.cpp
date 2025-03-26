#include <iostream>
#include <cmath>
#include <algorithm>

int main() {
    float x, y;

    std::cout << " x: ";
    std::cin >> x;
    std::cout << " y: ";
    std::cin >> y;

    float R = x * tan(acos(-1) / 2);

    float S = sqrt(x * x + y * y) / pow(2, log2(x));

    std::cout << "R = " << R << std::endl;
    std::cout << "S = " << S << std::endl;

    float C = std::max(R, S);

    std::cout << "C = " << C << std::endl;

    std::cout << "Press ENTER to exit....";
    system("pause");
    return 0;
}