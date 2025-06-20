#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <algorithm>
#include <random>

using namespace std;
using namespace sf;

// Глобальные константы
const int GRID_WIDTH = 10;
const int GRID_HEIGHT = 10;
const int CELL_SIZE = 60; // Размер ячейки пиксели
const int WINDOW_WIDTH = GRID_WIDTH * CELL_SIZE;
const int WINDOW_HEIGHT = GRID_HEIGHT * CELL_SIZE;

// Типы ячеек для логики и отрисовки
enum CellType {
    EMPTY,
    OBSTACLE,
    START,
    END,
    PATH,
    VISITED,
    OPEN
};

// Структура для представления ячейки на поле
struct Cell {
    int x, y;
    CellType type = EMPTY;

    // Поля для алгоритма A*
    int g = 0, h = 0, f = 0;
    Cell* parent = nullptr;

    Cell(int x_pos = 0, int y_pos = 0) : x(x_pos), y(y_pos) {}
};

// Вспомогательные функции

// Функция для проверки находится ли ячейка в пределах сетки
bool isValid(int x, int y) {
    return x >= 0 && x < GRID_WIDTH && y >= 0 && y < GRID_HEIGHT;
}

// Вычисление эвристики (манхэттенское расстояние)
int heuristic(const Cell& a, const Cell& b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}

// Функция для восстановления финального пути
void reconstructPath(Cell* end, vector<vector<Cell>>& grid) {
    Cell* current = end->parent;
    while (current != nullptr && current->parent != nullptr) {
        grid[current->x][current->y].type = PATH;
        current = current->parent;
    }
}

// Основной алгоритм A*
void a_star_algorithm(vector<vector<Cell>>& grid, Cell* start, Cell* end, RenderWindow& window);

// Функции для настройки поля

// Сброс поля к исходному состоянию (без пути, и т.д.)
void resetGrid(vector<vector<Cell>>& grid) {
    for (int i = 0; i < GRID_WIDTH; ++i) {
        for (int j = 0; j < GRID_HEIGHT; ++j) {
            grid[i][j].parent = nullptr;
            grid[i][j].g = 0;
            grid[i][j].h = 0;
            grid[i][j].f = 0;
            if (grid[i][j].type == VISITED || grid[i][j].type == OPEN || grid[i][j].type == PATH) {
                grid[i][j].type = EMPTY;
            }
        }
    }
}

// Настройка поля для Варианта 11
void setupVariant11Grid(vector<vector<Cell>>& grid, Cell*& start, Cell*& end) {
    // Сначала все ячейки пустые
    for (int i = 0; i < GRID_WIDTH; ++i) {
        for (int j = 0; j < GRID_HEIGHT; ++j) {
            grid[i][j] = Cell(i, j);
            grid[i][j].type = EMPTY;
        }
    }

    // Расставляем препятствия
    vector<pair<int, int>> obstacles = {
        {1,0}, {3,0}, {5,0},
        {2,1}, {6,1},
        {1,2}, {4,2},
        {3,3}, {7,3},
        {0,4}, {2,4}, {5,4}, {8,4},
        {1,5},
        {4,6}, {6,6},
        {3,7}, {7,7},
        {5,8}, {9,8},
        {4,9}
    };
    for (const auto& p : obstacles) {
        grid[p.first][p.second].type = OBSTACLE;
    }

    // старт и финиш
    grid[0][4].type = START;
    grid[9][9].type = END;
    start = &grid[0][4];
    end = &grid[9][9];

    resetGrid(grid); // Очищаем старые пути (если были)
}

// Настройка случайного поля
void setupRandomGrid(vector<vector<Cell>>& grid, Cell*& start, Cell*& end) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(0, GRID_WIDTH - 1);
    uniform_real_distribution<> wall_dist(0.0, 1.0);

    for (int i = 0; i < GRID_WIDTH; ++i) {
        for (int j = 0; j < GRID_HEIGHT; ++j) {
            grid[i][j] = Cell(i, j);
            if (wall_dist(gen) < 0.25) { // 25% шанс, что ячейка препятствие
                grid[i][j].type = OBSTACLE;
            }
            else {
                grid[i][j].type = EMPTY;
            }
        }
    }

    // Старт и финиш в случайных пустых ячейках
    int startX, startY, endX, endY;
    do {
        startX = distrib(gen);
        startY = distrib(gen);
    } while (grid[startX][startY].type == OBSTACLE);

    do {
        endX = distrib(gen);
        endY = distrib(gen);
    } while (grid[endX][endY].type == OBSTACLE || (startX == endX && startY == endY));

    grid[startX][startY].type = START;
    grid[endX][endY].type = END;
    start = &grid[startX][startY];
    end = &grid[endX][endY];

    resetGrid(grid);
}

// Функция отрисовки
void drawGrid(RenderWindow& window, const vector<vector<Cell>>& grid) {
    window.clear(Color::White);
    for (int i = 0; i < GRID_WIDTH; ++i) {
        for (int j = 0; j < GRID_HEIGHT; ++j) {
            RectangleShape cellShape(Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
            cellShape.setPosition(i * CELL_SIZE, j * CELL_SIZE);

            Color cellColor;
            switch (grid[i][j].type) {
            case EMPTY:    cellColor = Color(200, 200, 200); break; // Серый пустые
            case OBSTACLE: cellColor = Color(50, 50, 50);    break; // Темно-серый стены
            case START:    cellColor = Color::Blue;          break;
            case END:      cellColor = Color::Red;           break;
            case PATH:     cellColor = Color(147, 112, 219); break; // Фиолетовый
            case VISITED:  cellColor = Color(255, 105, 180); break; // Розовый
            case OPEN:     cellColor = Color(173, 216, 230); break; // Светло-голубой
            }
            cellShape.setFillColor(cellColor);
            window.draw(cellShape);
        }
    }
    window.display();
}

// main craft realization
int main() {
    RenderWindow window(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "A* Pathfinding");
    window.setFramerateLimit(60);

    vector<vector<Cell>> grid(GRID_WIDTH, vector<Cell>(GRID_HEIGHT));
    Cell* start = nullptr;
    Cell* end = nullptr;

    setupVariant11Grid(grid, start, end);

    cout << "A* Pathfinding Demo" << endl;
    cout << "Controls:" << endl;
    cout << " - SPACE: Start A* algorithm" << endl;
    cout << " - R: Generate a new random grid" << endl;
    cout << " - V: Load Variant 11 grid" << endl;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Space) {
                    cout << "Starting A* algorithm..." << endl;
                    a_star_algorithm(grid, start, end, window);
                }
                if (event.key.code == Keyboard::R) {
                    cout << "Generating random grid..." << endl;
                    setupRandomGrid(grid, start, end);
                }
                if (event.key.code == Keyboard::V) {
                    cout << "Loading Variant 11 grid..." << endl;
                    setupVariant11Grid(grid, start, end);
                }
            }
        }
        drawGrid(window, grid);
    }
    return 0;
}


// A*
void a_star_algorithm(vector<vector<Cell>>& grid, Cell* start, Cell* end, RenderWindow& window) {
    // Сброс состояний перед запуском
    resetGrid(grid);
    grid[start->x][start->y].type = START;
    grid[end->x][end->y].type = END;

    // Лямбда-функция для сравнения ячеек в очереди с приоритетом.
    // Позволяет извлекать ячейку с наименьшей стоимостью f.
    auto cmp = [](const Cell* left, const Cell* right) { return left->f > right->f; };
    priority_queue<Cell*, vector<Cell*>, decltype(cmp)> open_list(cmp);

    start->g = 0;
    start->h = heuristic(*start, *end);
    start->f = start->g + start->h;
    open_list.push(start);

    // отслеживание уже пройденных ячеек
    vector<vector<bool>> closed_list(GRID_WIDTH, vector<bool>(GRID_HEIGHT, false));

    // Возможные ходы
    int dx[] = { 0, 0, 1, -1 };
    int dy[] = { 1, -1, 0, 0 };

    while (!open_list.empty()) {
        // Проверка событий окна
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) window.close();
        }

        Cell* current = open_list.top();
        open_list.pop();

        if (current == end) {
            cout << "Path found!" << endl;
            reconstructPath(end, grid);
            // цвета старта и финиша
            grid[start->x][start->y].type = START;
            grid[end->x][end->y].type = END;
            return;
        }

        closed_list[current->x][current->y] = true;
        if (grid[current->x][current->y].type != START)
            grid[current->x][current->y].type = VISITED;

        for (int i = 0; i < 4; ++i) {
            int newX = current->x + dx[i];
            int newY = current->y + dy[i];

            if (isValid(newX, newY) && grid[newX][newY].type != OBSTACLE && !closed_list[newX][newY]) {
                Cell* neighbor = &grid[newX][newY];
                int tentative_g = current->g + 1;

                if (tentative_g < neighbor->g || neighbor->parent == nullptr) {
                    neighbor->parent = current;
                    neighbor->g = tentative_g;
                    neighbor->h = heuristic(*neighbor, *end);
                    neighbor->f = neighbor->g + neighbor->h;

                    open_list.push(neighbor);
                    if (neighbor->type != END)
                        neighbor->type = OPEN;
                }
            }
        }
        // Отрисовка каждого шага
        drawGrid(window, grid);
    }

    cout << "Path not found." << endl;
}