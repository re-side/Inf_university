#include <iostream>
#include <cmath>

using namespace std;

double upperY(double x) {
    if (x <= 4) return 2 * x - 4;
    else return -2 * x + 12;
}

double lowerY(double x) {
    if (x <= 4) return -2 * x + 4;
    else return 2 * x - 12;
}

double simpson2D(int n) {
    double a = 2.0, b = 6.0;
    double h = (b - a) / n;
    double integral = 0.0;

    for (int i = 0; i <= n; i++) {
        double x = a + i * h;
        double y1 = upperY(x);
        double y2 = lowerY(x);
        double height = y1 - y2;
        integral += height * (i == 0 || i == n ? 1 : (i % 2 == 0 ? 2 : 4));
    }

    integral *= (h / 3.0);
    return integral;
}

int main() {
    setlocale(LC_ALL, "Russian");
    int n_values[] = { 10, 50, 100, 1000, 10000, 100000, 1000000};
    for (int n : n_values) {
        double result = simpson2D(n);
        cout << "N = " << n << ", Площадь методом Симпсона: " << result << endl;
    }
    return 0;
}
