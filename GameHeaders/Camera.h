//
// Created by ultragreed on 10/9/22.
//

#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

class Camera {
public:
    Camera(int screenWidth, int screenHeight, const std::vector<std::vector<int>> &mazeWalls);
    void Draw(double playerX, double playerY, double playerAng, sf::RenderWindow &window);
private:
    std::vector<std::vector<int>> _mazeWalls;
    int _screenHeight, _screenWidth;
    double _heightMultiplier = 1;
    double _fieldOfView = acos(-1) / 3;
    double _step = 0.05;
    double _maxRange = 5;

    double RayCast(double startX, double startY, double angle);
};
