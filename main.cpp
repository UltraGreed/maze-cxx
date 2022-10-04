#include <iostream>
#include "generateMaze.h"

int main(){
    int xLength, yLength;
    std::cin >> xLength >> yLength;
    generateMaze(xLength, yLength);

    return 0;
}