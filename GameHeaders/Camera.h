//
// Created by ultragreed on 10/9/22.
//

#pragma once

#include <SFML/Graphics.hpp>

class Camera {
public:
    Camera(int screenWidth, int screenHeight, int **maze, int mazeWidth, int mazeHeight, double cameraHeightMultiplier,
           double fieldOfView, double cameraStep, double maxRange, double brightness);

    void Draw(double playerX, double playerY, double playerAng, sf::RenderWindow &window);

private:
    int **_maze;
    int _screenHeight, _screenWidth;
    int _mazeHeight, _mazeWidth;
    double _heightMultiplier, _fieldOfView;
    double _rayStep, _maxRange;
    double _brightness;

    double RayCast(double startX, double startY, double angle);
};
