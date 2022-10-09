//
// Created by ultragreed on 10/9/22.
//

#ifndef THEBESTLABYRINTH_CAMERA_H
#define THEBESTLABYRINTH_CAMERA_H

#include <cmath>
#include <string>
#include <vector>
#include <iostream>
#include <ncurses.h>

class Camera {
public:
    Camera(int screenWidth, int screenHeight, const std::vector<std::string> &map) {
        _screenWidth = screenWidth;
        _screenHeight = screenHeight;
        _map = map;
        initscr();
        cbreak();
        noecho();
    }

    void Update(double newX, double newY, double newAngle) {
        Draw(newX, newY, newAngle);
    }

private:
    std::vector<std::string> _map;
    int _screenHeight, _screenWidth;
    double _heightMultiplier = 0.75;
    double _fieldOfView = acos(-1);
    double _step = 0.1;
    double _maxRange = 100;


    double RayCast(double startX, double startY, double angle) {
        double wallDistance = _maxRange;
        bool isHitWall = false;
        double x = startX;
        double y = startY;
        while (wallDistance < _maxRange && !isHitWall) {
            x += _step * cos(angle);
            y += _step * sin(angle);
            if (_map[(int) round(x)][(int) round(y)] == '#') {
                isHitWall = true;
                wallDistance = sqrt(pow(startX - x, 2) + pow(startY - y, 2));
            }
        }
        return wallDistance;
    }

    void Draw(double playerX, double playerY, double playerAng) {
        char16_t screen[_screenWidth][_screenHeight];
        for (int screenX = 0; screenX < _screenWidth; screenX++) {
            double stepAng = 1.0 / _screenWidth;
            double angle = playerAng + screenX * stepAng;
            double distance = RayCast(playerX, playerY, angle);
            for (int screenY = 0; screenY < _screenHeight; screenY++) {
                if (_screenHeight * (0.5 - _heightMultiplier / distance) < screenY &&
                    screenY > _screenHeight * (0.5 + _heightMultiplier / distance))
                    screen[screenX][screenY] = u'■';
                else
                    screen[screenX][screenY] = u'░';
            }
        }
        for (int screenY = 0; screenY < _screenHeight; screenY++) {
            const char16_t *s;
            for (int screenX = 0; screenX < _screenWidth; screenX++){
                s += screen[screenX][screenY];
            }
            printw(s);
        }


    }

};

#endif //THEBESTLABYRINTH_CAMERA_H
