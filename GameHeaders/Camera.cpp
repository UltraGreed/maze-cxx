//
// Created by ultragreed on 10/14/22.
//

#include <cmath>
#include <SFML/Graphics.hpp>

#include "Camera.h"

Camera::Camera(int screenWidth, int screenHeight, int **maze, int mazeWidth, int mazeHeight,
               double cameraHeightMultiplier, double fieldOfView, double cameraStep, double maxRange,
               double brightness) {
    _screenWidth = screenWidth;
    _screenHeight = screenHeight;
    _maze = maze;
    _mazeWidth = mazeWidth;
    _mazeHeight = mazeHeight;
    _heightMultiplier = cameraHeightMultiplier;
    _fieldOfView = fieldOfView;
    _rayStep = cameraStep;
    _maxRange = maxRange;
    _brightness = brightness;
}

void Camera::Draw(double playerX, double playerY, double playerAng, sf::RenderWindow &window) {
    for (int screenX = 0; screenX < _screenWidth; screenX++) {
        double stepAng = _fieldOfView / _screenWidth;
        double angle = playerAng - _fieldOfView / 2 + screenX * stepAng;
        double distance = RayCast(playerX + 0.5, playerY + 0.5, angle);
        sf::Vertex line[] =
                {
                        sf::Vertex(sf::Vector2f(
                                screenX, 0.5 * _screenHeight * (1 - _heightMultiplier / distance))
                        ),
                        sf::Vertex(sf::Vector2f(
                                screenX, 0.5 * _screenHeight * (1 + _heightMultiplier / distance))
                        ),
                };

        int rgb = 255 * (1 - distance / _maxRange) * _brightness;
        sf::Color wallColor = sf::Color(rgb, rgb, rgb);

        line[0].color = wallColor;
        line[1].color = wallColor;

        window.draw(line, 2, sf::Lines);
    }
}


double Camera::RayCast(double startX, double startY, double angle) {
    double wallDistance = 0;
    bool isHitWall = false;
    double x = startX;
    double y = startY;
    while (!isHitWall && wallDistance < _maxRange) {
        x += _rayStep * cos(angle);
        y += _rayStep * sin(angle);
        wallDistance = sqrt(pow(startX - x, 2) + pow(startY - y, 2));
        if (y >= 0 && y < _mazeHeight && x >= 0 && x < _mazeWidth && _maze[(int) y][(int) x]) {
            isHitWall = true;
        }
    }
    if (isHitWall)
        return wallDistance;
    else
        return 10000.0;
}

