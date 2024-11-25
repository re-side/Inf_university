#include <SFML/Graphics.hpp>
#include <functional>
#include <cmath>
#include <iostream> 


// Функция для отрисовки графика
void drawGraph(sf::RenderWindow& window, std::function<float(float)> func, float xMin, float xMax, float scaleX, float scaleY, sf::Color color) {
    sf::VertexArray lines(sf::LinesStrip);
    for (float x = xMin; x <= xMax; x += 0.1f) {
        float y = func(x);
        float screenX = 400 + x * scaleX;
        float screenY = 300 - y * scaleY;
        lines.append(sf::Vertex(sf::Vector2f(screenX, screenY), color));
    }
    window.draw(lines);
}

int main() {
    // Создание окна
    sf::RenderWindow window(sf::VideoMode(800, 600), "APP dlya graphicof");
    window.setFramerateLimit(60);

    // Переменная для хранения пользовательской точки
    sf::CircleShape userPoint(5); // Радиус 5 пикселей
    userPoint.setFillColor(sf::Color::Red);
    bool userPointExists = false; // Переменная для проверки существования пользовательской точки

    // Загрузка шрифта.
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "oshibka" << std::endl; 
        return -1;
    }

    // Текст для отображения координат точки
    sf::Text coordinatesText;
    coordinatesText.setFont(font);
    coordinatesText.setCharacterSize(20);
    coordinatesText.setFillColor(sf::Color::White);
    coordinatesText.setPosition(10, 10);
    coordinatesText.setString("Coordinates: (No)");


    // Оси X и Y
    sf::VertexArray xAxis(sf::Lines, 2);
    xAxis[0].position = sf::Vector2f(50, 300); // Начало оси X
    xAxis[0].color = sf::Color::White; // Цвет оси
    xAxis[1].position = sf::Vector2f(750, 300); // Конец оси X
    xAxis[1].color = sf::Color::White;

    sf::VertexArray yAxis(sf::Lines, 2);
    yAxis[0].position = sf::Vector2f(400, 50); // Начало оси Y
    yAxis[0].color = sf::Color::White; // Цвет оси
    yAxis[1].position = sf::Vector2f(400, 550); // Конец оси Y
    yAxis[1].color = sf::Color::White;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // Проверка клика мышкой
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    // Получение позиции клика
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                    // Преобразование экранных координат в "математические"
                    float mathX = (mousePos.x - 400) / 50.0f; // Масштаб 50 по X
                    float mathY = -(mousePos.y - 300) / 50.0f; // Масштаб 50 по Y

                    // Установка новой пользовательской точки
                    userPoint.setPosition(mousePos.x - userPoint.getRadius(), mousePos.y - userPoint.getRadius());
                    userPointExists = true; // Помечаем, что точка существует

                    std::string zone;
                    float tolerance = 0.01;
                    float y1 = std::abs(mathX) - mathX; // y = |x| - x
                    float y2 = -std::abs(mathX);       // y = -|x|

                    if (std::abs(mathY - y1) < tolerance || std::abs(mathY - y2) < tolerance) {
                        zone = "Граница";
                    }
                    else if (mathY > y1) {
                        zone = "2";
                    }
                    else if (mathY < y2) { 
                        zone = "1";
                    }
                    else if (mathY > y2 && mathY < y1 && mathX < 0) { 
                        zone = "3";
                    }
                    else if (mathY < y1 && mathY > y2 && mathX >= 0) { 
                        zone = "4";
                    }
                    else {                              
                        zone = "oshibka";
                    }


                    // Обновление текста с координатами точки и зоны
                    coordinatesText.setString("Coordinates: (" + std::to_string(mathX) + ", " + std::to_string(mathY) + ") Zone: " + zone);
                }
            }
        }

        // Очистка экрана
        window.clear();

        // Отрисовка осей и текста с изначальными координатами
        window.draw(xAxis);
        window.draw(yAxis);
        window.draw(coordinatesText);

        // Отрисовка графика y1 = |x| - x
        drawGraph(window, [](float x) { return std::abs(x) - x; }, -10, 10, 50, 50, sf::Color::Red);

        // Отрисовка графика y2 = -|x|
        drawGraph(window, [](float x) { return -std::abs(x); }, -10, 10, 50, 50, sf::Color::Blue);

        // Отрисовка пользовательской точки, если она существует
        if (userPointExists) {
            window.draw(userPoint);
        }

        // Отображение нового кадра
        window.display();
    }

    return 0;
}
