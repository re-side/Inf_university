#include <iostream>
#include <cmath>

using namespace std;

inline double upperY(double x) {
    return 2 - fabs(x);
}

inline double lowerY(double x) {
    return -upperY(x);
}

double simpson2D(int n) {
    double a = -2.0, b = 2.0;
    double h = (b - a) / n;
    double integral = 0.0;

    for (int i = 0; i <= n; i++) {
        double x = a + i * h;
        double y1 = upperY(x);
        double y2 = lowerY(x);
        integral += (y1 - y2) * (i == 0 || i == n ? 1 : (i % 2 == 0 ? 2 : 4));
    }

    integral *= (h / 3.0);
    return integral;
}

int main() {
    int n = 1000;
    double result = simpson2D(n);
    cout << "Площадь фигуры методом Симпсона: " << result << endl;
    return 0;
}