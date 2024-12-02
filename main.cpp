#include <SFML/Graphics.hpp>
#include <vector>

using namespace sf;

int main() {
    const int windowWidth = 500;
    const int windowHeight = 500;
    const int cellSize = 50;
    const int n = 10;

    RenderWindow window(VideoMode(windowWidth, windowHeight), "Matrix");

    int coords[][2] = {
        {0, 1}, {0, 3}, {0, 5}, {0, 7}, {0, 9},
        {1, 2}, {1, 4}, {1, 6}, {1, 8},
        {2, 3}, {2, 5}, {2, 7}, {2, 9},
        {3, 4}, {3, 6}, {3, 8},
        {4, 5}, {4, 7}, {4, 9},
        {5, 6}, {5, 8},
        {6, 7}, {6, 9},
        {7, 8},
        {8, 9}
    };

    RectangleShape cell(Vector2f(cellSize, cellSize));
    RectangleShape gridLine(Vector2f(1, windowHeight)); // Линия сетки по вертикали
    gridLine.setFillColor(Color::Black);
    RectangleShape gridLine2(Vector2f(windowWidth, 1)); // Линия сетки по горизонтали
    gridLine2.setFillColor(Color::Black);

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }

        window.clear();

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                cell.setPosition(j * cellSize, i * cellSize);
                bool isGreen = false;
                for (const auto& coord : coords) {
                    if (coord[0] == i && coord[1] == j) {
                        isGreen = true;
                        break;
                    }
                }
                cell.setFillColor(isGreen ? Color::Green : Color::White);
                window.draw(cell);
            }
        }

        // Рисуем сетку
        for (int i = 1; i < n; ++i) {
            gridLine.setPosition(i * cellSize, 0);
            window.draw(gridLine);
            gridLine2.setPosition(0, i * cellSize);
            window.draw(gridLine2);
        }

        window.display();
    }

    return 0;
}

