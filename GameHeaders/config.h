//
// Created by ultragreed on 10/10/22.
//

#pragma once

namespace config {
    // player config
    double const playerSpeed = 1;
    double const playerAngularSpeed = 2;
    double const playerX = 8;
    double const playerY = -2;
    double const playerAngle = asin(1);
    // screen config
    int const screenHeight = 1080;
    int const screenWidth = 1920;
    // maze config
    int const mazeHeight = 15;
    int const mazeWidth = 15;
    // camera config
    double maxRange = 15;
    double rayStep = 0.01;
    double fieldOfView = acos(-1) / 3;
    double cameraHeightMultiplier = 1;
    double brightness = 0.8;
}
