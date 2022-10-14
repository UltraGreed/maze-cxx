//
// Created by ultragreed on 10/9/22.
//

#pragma once
#include <queue>
#include <cmath>


class Player {
public:
    Player(double speed, double speedAng);

    void PutMoveInQueue(char direction);

    void Update(double deltaTime);

    double GetX() const;

    double GetY() const;

    double GetAngle() const;

    void Debug() const;

private :
    double _speed, _speedAng; // distance per second
    double _x = 8;
    double _y = 16;
    double _angle = asin(-1);
    double _progress = 0;
    char _state = ' '; // ' ' - idle
    // move: w - forward, e - right, s - back, q - left
    // rotate: d - right, a - left
    std::queue<char> _stateQueue;

    void ContinueMovement(char state, double deltaTime);

    bool IsIdle() const;
};
