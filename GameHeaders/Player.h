//
// Created by ultragreed on 10/9/22.
//

#pragma once

#include <queue>
#include <cmath>


class Player {
public:
    Player(double speed, double speedAng, int **maze, int mazeWidth, int mazeHeight, double playerX, double playerY,
           double playerAngle);

    void PutMoveInQueue(char direction);

    void Update(double deltaTime);

    double GetX() const;

    double GetY() const;

    double GetAngle() const;

    void Debug() const;

private :
    double _speed, _speedAng; // distance per second
    int _mazeWidth, _mazeHeight;
    int **_maze;
    double _x, _y;
    double _angle;
    double _progress = 0;
    char _state = ' '; // ' ' - idle
    // move: w - forward, e - right, s - back, q - left
    // rotate: d - right, a - left
    std::queue<char> _stateQueue;

    void ContinueMovement(char state, double deltaTime);

    bool IsIdle() const;
};
