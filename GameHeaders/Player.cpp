//
// Created by ultragreed on 10/14/22.
//

#include <cmath>
#include <string>

#include "Player.h"

Player::Player(double speed, double speedAng, int **maze, int mazeWidth, int mazeHeight, double playerX, double playerY,
               double playerAngle) {
    _speed = speed;
    _speedAng = speedAng;
    _mazeHeight = mazeHeight;
    _mazeWidth = mazeWidth;
    _maze = maze;
    _x = playerX;
    _y = playerY;
    _angle = playerAngle;
}

void Player::PutMoveInQueue(char direction) {
    if (_stateQueue.empty())
        _stateQueue.push(direction);
}

void Player::Update(double deltaTime) {
    if (IsIdle() && !_stateQueue.empty()) {
        _state = _stateQueue.front();
        _stateQueue.pop();
        _progress = 0;
    }
    if (!IsIdle()) {
        ContinueMovement(_state, deltaTime);
        if (_progress == 1)
            _state = ' ';
    }
}

double Player::GetX() const { return _x; }

double Player::GetY() const { return _y; }

double Player::GetAngle() const { return _angle; }

void Player::Debug() const {
    printf("_x: %f, _y: %f\n_state: %c\n_angle: %f\n_progress: %f\n", _x, _y, _state, _angle, _progress);
    for (int y = 0; y < _mazeHeight; y++) {
        for (int x = 0; x < _mazeWidth; x++)
            if (_x == x && _y == y)
                printf("_");
            else
                printf("%d", _maze[y][x]);
        printf("\n");
    }
    printf("\nDone");
}


void Player::ContinueMovement(char state, double deltaTime) {
    double angle;
    if (state == 'w' || state == 'a' || state == 's' || state == 'd') { // move
        double step = deltaTime * _speed;
        step = step < 1 - _progress ? step : 1 - _progress;
        if (state == 'w') // forward
            angle = _angle;
        if (state == 'a') // right
            angle = _angle - asin(1);
        if (state == 's') // back
            angle = _angle - acos(-1);
        if (state == 'd') // left
            angle = _angle + asin(1);
        _x += cos(angle) * step;
        _y += sin(angle) * step;
        _progress += step;
    } else if (state == 'e' || state == 'q') {
        double stepAng = deltaTime * _speedAng;
        stepAng = stepAng < 1 - _progress ? stepAng : 1 - _progress;
        if (state == 'e')
            angle = stepAng * asin(1);
        if (state == 'q')
            angle = stepAng * asin(-1);
        _angle += angle;
        _progress += stepAng;
    }
}


bool Player::IsIdle() const { return _state == ' '; }
