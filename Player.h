//
// Created by ultragreed on 10/9/22.
//

#ifndef THEBESTLABYRINTH_PLAYER_H
#define THEBESTLABYRINTH_PLAYER_H

#include <cmath>
#include <queue>
#include <string>

class Player {
public:
    Player(double x, double y, double angle, double speed, double speedAng) {
        _x = x;
        _y = y;
        _angle = angle;
        _speed = speed;
        _speedAng = speedAng;
    }

    void PutMoveInQueue(char direction) {
        if (_stateQueue.empty())
            _stateQueue.push(direction);
    }

    void Update(double deltaTime) {
        if (IsIdle() && !_stateQueue.empty()) {
            _state = _stateQueue.front();
            _stateQueue.pop();
            _progress = 0;
        }
        if (!IsIdle()) {
            double step = deltaTime * _speed;
            step = step < 1 - _progress ? step : 1 - _progress;
            double stepAng = deltaTime * _speedAng;
            stepAng = stepAng < 1 - _progress ? step : 1 - _progress;
            ContinueMovement(_state, step, stepAng);
            _progress += step;
            if (_progress == 1)
                _state = ' ';
        }
    }

    void Debug() const {
        printf("_x: %f, _y: %f\n_state: %c\n_angle: %f\n_progress: %f\n", _x, _y, _state, _angle, _progress);
    }

private :
    double _x, _y, _angle, _speed, _speedAng; // distance per second
    double _progress = 0;
    double const _fieldOfView = acos(-1) / 3;
    int const _accuracy = 1000;
    char _state = ' '; // ' ' - idle
    // move: w - forward, e - right, s - back, q - left
    // rotate: d - right, a - left
    std::queue<char> _stateQueue;

    void ContinueMovement(char state, double step, double stepAng) {
        double angle;
        if (state == 'w' || state == 'q' || state == 's' || state == 'e') { // move
            if (state == 'w') // forward
                angle = _angle;
            if (state == 'q') // right
                angle = _angle + asin(1);
            if (state == 's') // back
                angle = _angle - acos(-1);
            if (state == 'e') // left
                angle = _angle - asin(1);
            _x += cos(angle) * step;
            _y += sin(angle) * step;
        } else if (state == 'd' || state == 'a') {
            if (state == 'd')
                angle = stepAng * asin(1);
            if (state == 'a')
                angle = stepAng * sin(-1);
            _angle = angle;
        }
    }


    bool IsIdle() const { return _state == ' '; }
};


#endif //THEBESTLABYRINTH_PLAYER_H
