#include <iostream>
#include <cmath>
using namespace std;

double upperY(double x) {
    return min(-2 * x + 12, 2 * x - 4);
}

double lowerY(double x) {
    return max(2 * x - 12, -2 * x + 4);
}

double simpson2D(int n) {
    double a_x = 4.0;
    double b_x = 6.0;
    double h_x = (b_x - a_x) / n;
    double totalArea = 0.0;

    for (int i = 0; i <= n; ++i) {
        double x = a_x + i * h_x;
        double a_y = lowerY(x);
        double b_y = upperY(x);
        if (a_y >= b_y) continue;
        totalArea += (b_y - a_y) * h_x;
    }
    return totalArea;
}

int main() {
    setlocale(LC_ALL, "Russian");
    int n = 1000;
    cout << "Метод Симпсона: " << simpson2D(n) << endl;
    return 0;
}