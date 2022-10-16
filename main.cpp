#include "GameHeaders/GameState.h"

int main() {
    GameState gameState;

    bool isWorking = true;
    while (isWorking) {
        isWorking = gameState.Update();
    }

    return 0;
}


