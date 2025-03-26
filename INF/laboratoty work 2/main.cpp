#include <SFML/Graphics.hpp>
#include <functional>
#include <cmath>
#include <iostream> 


// ������� ��� ��������� �������
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
    // �������� ����
    sf::RenderWindow window(sf::VideoMode(800, 600), "APP dlya graphicof");
    window.setFramerateLimit(60);

    // ���������� ��� �������� ���������������� �����
    sf::CircleShape userPoint(5); // ������ 5 ��������
    userPoint.setFillColor(sf::Color::Red);
    bool userPointExists = false; // ���������� ��� �������� ������������� ���������������� �����

    // �������� ������.
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "oshibka" << std::endl; 
        return -1;
    }

    // ����� ��� ����������� ��������� �����
    sf::Text coordinatesText;
    coordinatesText.setFont(font);
    coordinatesText.setCharacterSize(20);
    coordinatesText.setFillColor(sf::Color::White);
    coordinatesText.setPosition(10, 10);
    coordinatesText.setString("Coordinates: (No)");


    // ��� X � Y
    sf::VertexArray xAxis(sf::Lines, 2);
    xAxis[0].position = sf::Vector2f(50, 300); // ������ ��� X
    xAxis[0].color = sf::Color::White; // ���� ���
    xAxis[1].position = sf::Vector2f(750, 300); // ����� ��� X
    xAxis[1].color = sf::Color::White;

    sf::VertexArray yAxis(sf::Lines, 2);
    yAxis[0].position = sf::Vector2f(400, 50); // ������ ��� Y
    yAxis[0].color = sf::Color::White; // ���� ���
    yAxis[1].position = sf::Vector2f(400, 550); // ����� ��� Y
    yAxis[1].color = sf::Color::White;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // �������� ����� ������
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    // ��������� ������� �����
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                    // �������������� �������� ��������� � "��������������"
                    float mathX = (mousePos.x - 400) / 50.0f; // ������� 50 �� X
                    float mathY = -(mousePos.y - 300) / 50.0f; // ������� 50 �� Y

                    // ��������� ����� ���������������� �����
                    userPoint.setPosition(mousePos.x - userPoint.getRadius(), mousePos.y - userPoint.getRadius());
                    userPointExists = true; // ��������, ��� ����� ����������

                    std::string zone;
                    float tolerance = 0.01;
                    float y1 = std::abs(mathX) - mathX; // y = |x| - x
                    float y2 = -std::abs(mathX);       // y = -|x|

                    if (std::abs(mathY - y1) < tolerance || std::abs(mathY - y2) < tolerance) {
                        zone = "�������";
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


                    // ���������� ������ � ������������ ����� � ����
                    coordinatesText.setString("Coordinates: (" + std::to_string(mathX) + ", " + std::to_string(mathY) + ") Zone: " + zone);
                }
            }
        }

        // ������� ������
        window.clear();

        // ��������� ���� � ������ � ������������ ������������
        window.draw(xAxis);
        window.draw(yAxis);
        window.draw(coordinatesText);

        // ��������� ������� y1 = |x| - x
        drawGraph(window, [](float x) { return std::abs(x) - x; }, -10, 10, 50, 50, sf::Color::Red);

        // ��������� ������� y2 = -|x|
        drawGraph(window, [](float x) { return -std::abs(x); }, -10, 10, 50, 50, sf::Color::Blue);

        // ��������� ���������������� �����, ���� ��� ����������
        if (userPointExists) {
            window.draw(userPoint);
        }

        // ����������� ������ �����
        window.display();
    }

    return 0;
}
