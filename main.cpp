#include <iostream>
#include <chrono>
#include "cstdio"
#include "generateMaze.h"
#include "keyRead.h"
#include "Player.h"


int main() {
    int const screenHeight = 1080;
    int const screenWidth = 1920;



    bool isWorking = true;
    double startX = 0;
    double startY = 0;
    double startAngle = asin(1);
    double playerSpeed = 1;
    double playerAngularSpeed = 1;
    Player player(startX, startY, startAngle, playerSpeed, playerAngularSpeed);
    auto tp1 = std::chrono::system_clock::now();
    auto tp2 = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsedTime{};
    double fElapsedTime;
    while (isWorking) {
        tp1 = std::chrono::system_clock::now();
        elapsedTime = tp1 - tp2;
        tp2 = tp1;
        fElapsedTime = elapsedTime.count();
        int pressedKey = keyRead();
        if (pressedKey == 27)
            isWorking = false;
        else if (pressedKey == 87 || pressedKey == 119)
            player.PutMoveInQueue('w'); // move forward
        else if (pressedKey == 69 || pressedKey == 101)
            player.PutMoveInQueue('e'); // move right
        else if (pressedKey == 83 || pressedKey == 115)
            player.PutMoveInQueue('s'); // move back
        else if (pressedKey == 81 || pressedKey == 113)
            player.PutMoveInQueue('q'); // move right
        else if (pressedKey == 68 || pressedKey == 100)
            player.PutMoveInQueue('d'); // rotate right
        else if (pressedKey == 65 || pressedKey == 97)
            player.PutMoveInQueue('a'); // rotate left
        else if (pressedKey == 32)
            player.Debug();
        else if (pressedKey) {
            printf("Unhandled key input: %d\n", pressedKey);
        }
        player.Update(fElapsedTime);
    }

    return 0;
}


