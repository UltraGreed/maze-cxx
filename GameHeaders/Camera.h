//
// Created by ultragreed on 10/9/22.
//

#pragma once

#include <SFML/Graphics.hpp>

class Camera {
public:
    Camera(int screenWidth, int screenHeight, int **mazeMap, int mazeWidth, int mazeHeight, double cameraHeightMultiplier,
           double fieldOfView, double maxRange, double brightness);

    void Draw(double playerX, double playerY, double playerAng, sf::RenderWindow &window);

private:
    int **_mazeMap;
    int _screenHeight, _screenWidth;
    int _mazeHeight, _mazeWidth;
    double _heightMultiplier, _fieldOfView;
    double _maxRange;
    double _brightness;

    const int RAW = 0;
    const int SPACE = 1;
    const int INNER_WALL = 2;
    const int OUTER_WALL = 3;

    double RayCast(double startX, double startY, double angle);
};
