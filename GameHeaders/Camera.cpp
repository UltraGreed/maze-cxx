//
// Created by ultragreed on 10/14/22.
//

#include <cmath>
#include <SFML/Graphics.hpp>

#include "Camera.h"

Camera::Camera(int screenWidth, int screenHeight, int **mazeMap, int mazeWidth, int mazeHeight,
               double cameraHeightMultiplier, double fieldOfView, double maxRange,
               double brightness) {
    _screenWidth = screenWidth;
    _screenHeight = screenHeight;
    _mazeMap = mazeMap;
    _mazeWidth = mazeWidth;
    _mazeHeight = mazeHeight;
    _heightMultiplier = cameraHeightMultiplier;
    _fieldOfView = fieldOfView;
    _maxRange = maxRange;
    _brightness = brightness;
}

void Camera::Draw(double playerX, double playerY, double playerAng, sf::RenderWindow &window) {
    double stepAng = _fieldOfView / _screenWidth;
    for (int screenX = 0; screenX < _screenWidth; screenX++) {
        double angle = playerAng - _fieldOfView / 2 + screenX * stepAng;
        double rayCastDistance = RayCast(playerX + 0.5, playerY + 0.5, angle);
        if (rayCastDistance == _maxRange)
            continue;
        double trueDistance = rayCastDistance * cos(_fieldOfView / 2 - screenX * stepAng);
        sf::Vertex line[] =
                {
                        sf::Vertex(sf::Vector2f(
                                screenX, 0.5 * _screenHeight * (1 - _heightMultiplier / trueDistance))
                        ),
                        sf::Vertex(sf::Vector2f(
                                screenX, 0.5 * _screenHeight * (1 + _heightMultiplier / trueDistance))
                        ),
                };

        int rgb =  255 * (rayCastDistance / _maxRange < 1 ? 1 - rayCastDistance / _maxRange : 0) * _brightness;
        sf::Color wallColor = sf::Color(rgb, rgb, rgb);

        line[0].color = wallColor;
        line[1].color = wallColor;

        window.draw(line, 2, sf::Lines);
    }
}


double Camera::RayCast(double startX, double startY, double angle) {
    double cosA = cos(angle);
    double sinA = sin(angle);

    double x = startX;
    double y = startY;

    bool isHitWall = false;
    double wallDistance = 0;
    while (!isHitWall && wallDistance < _maxRange) {
        double deltaLx, deltaLy; // distance from current point to next cell border

        // calculating distance from this point to closest x cell border
        if (int(x) == x || cosA >= 0 && x >= 0 || cosA < 0 && x < 0)
            deltaLx = (1 - std::abs(int(x) - x)) / std::abs(cosA);
        else
            deltaLx = (int(x) - x) / cosA;

        // calculating distance from this point to closest y cell border
        if (int(y) == y || sinA >= 0 && y >= 0 || sinA < 0 && y < 0)
            deltaLy = (1 - std::abs(int(y) - y)) / std::abs(sinA);
        else
            deltaLy = (int(y) - y) / sinA;

        // we make lesser step every iteration
        double stepL = std::min(deltaLx, deltaLy);

        x += stepL * cosA;
        y += stepL * sinA;
        wallDistance += stepL;

        // fix needed to select correct cell from map
        int cellX = int(x) - ((x == int(x) && cosA < 0 || x < 0) ? 1 : 0);
        int cellY = int(y) - ((y == int(y) && sinA < 0 || y < 0) ? 1 : 0);

        // check if the cell is inside maze and contains a wall
        if (cellY >= 0 && cellY < _mazeHeight && cellX >= 0 && cellX < _mazeWidth) {
            int cellValue = _mazeMap[cellY][cellX];
            if (cellValue == INNER_WALL || cellValue == OUTER_WALL)
                isHitWall = true;
        }
    }

    if (isHitWall) {
        return wallDistance;
    } else
        return _maxRange;
}

