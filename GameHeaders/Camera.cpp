//
// Created by ultragreed on 10/14/22.
//

#include <cmath>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Camera.h"

Camera::Camera(int screenWidth, int screenHeight, const std::vector<std::vector<int>> &mazeWalls) {
    _screenWidth = screenWidth;
    _screenHeight = screenHeight;
    _mazeWalls = mazeWalls;
}

void Camera::Draw(double playerX, double playerY, double playerAng, sf::RenderWindow &window) {
    for (int screenX = 0; screenX < _screenWidth; screenX++) {
        double stepAng = _fieldOfView / _screenWidth;
        double angle = playerAng - _fieldOfView / 2 + screenX * stepAng;
        double distance = RayCast(playerX, playerY, angle);
        sf::Vertex line[] =
                {
                        sf::Vertex(sf::Vector2f(
                                screenX, 0.5 * _screenHeight * (1 - _heightMultiplier / distance))
                        ),
                        sf::Vertex(sf::Vector2f(
                                screenX, 0.5 * _screenHeight * (1 + _heightMultiplier / distance))
                        ),
                };
        sf::Color wallColor = sf::Color(255, 255, 255);
        if (distance < _maxRange / 10) {
            wallColor = sf::Color(255, 255, 255);
        } else if (distance < _maxRange / 5) {
            wallColor = sf::Color(245, 245, 245);
        } else if (distance < _maxRange / 2) {
            wallColor = sf::Color(235, 235, 235);
        } else {
            wallColor = sf::Color(225, 225, 225);
        }
        //printf("%f",  _screenHeight * _heightMultiplier / distance);
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
    while (wallDistance < _maxRange && !isHitWall) {
        x += _step * cos(angle);
        y += _step * sin(angle);
        wallDistance = sqrt(pow(startX - x, 2) + pow(startY - y, 2));
        for (std::vector<int> &wall: _mazeWalls) {
            if (wall.at(1) == (int) round(y) && wall.at(0) == (int) round(x)) {
                isHitWall = true;
            }
        }
    }
    if (isHitWall)
        return wallDistance;
    else
        return 10000.0;
}

