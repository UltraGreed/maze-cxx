//
// Created by ultragreed on 10/10/22.
//

#pragma once

#include <vector>
#include <random>

class Maze {
public:
    Maze(int mazeWidth, int mazeHeight);

    int **GetMazeMap();
private:
    std::random_device _rd;
    std::mt19937 *_gen;
    int **_mazeMap;

    const int RAW = 0;
    const int SPACE = 1;
    const int INNER_WALL = 2;
    const int OUTER_WALL = 3;

    const int TOP_WALL_WITHOUT_SPACE = 0;
    const int RIGHT_WALL_WITHOUT_SPACE = 1;
    const int BOTTOM_WALL_WITHOUT_SPACE = 2;
    const int LEFT_WALL_WITHOUT_SPACE = 3;

    int GetRandomIntFromRange(int rangeStart, int rangeEnd);

    void GenerateChamber(int startX, int startY, int endX, int endY);

    void GenerateMaze(int lengthX, int lengthY);
};