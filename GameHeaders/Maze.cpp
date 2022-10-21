//
// Created by ultragreed on 10/14/22.
//
#include <fstream>
#include <vector>
#include <random>

#include "Maze.h"

Maze::Maze(int mazeWidth, const int mazeHeight) {
    _gen = new std::mt19937(_rd());
    _mazeMap = new int *[mazeHeight];
    for (int i = 0; i < mazeHeight; i++)
        _mazeMap[i] = new int[mazeWidth];

    GenerateMaze(mazeWidth, mazeHeight);
}

int **Maze::GetMazeMap() {
    return _mazeMap;
}

int Maze::GetRandomIntFromRange(int rangeStart, int rangeEnd) {
    std::uniform_int_distribution<int> rangeDist(rangeStart, rangeEnd);
    return rangeDist(*_gen);
}

void Maze::GenerateChamber(int startX, int startY, int endX, int endY) {
    if (endX - startX == 0 || endY - startY == 0 || endX - startX <= 1 && endY - startY <= 1)
        // exit if no space left
        return;

    // randomizing split coordinates
    int splitY, splitX;

    bool isVerticalSplit = (endX - startX >= 2);
    bool isHorizontalSplit = (endY - startY >= 2);

    // this chamber horizontal walls generation
    if (isHorizontalSplit) {
        splitY = GetRandomIntFromRange(startY + 1, endY - 1);
        for (int x = startX; x < endX + 1; x++) {
            int wallX = x;
            int wallY = splitY;

            // leave space if wall would be an obstacle for another wall's door
            if (_mazeMap[wallY][wallX - 1] == SPACE || _mazeMap[wallY][wallX + 1] == SPACE)
                _mazeMap[wallY][wallX] = RAW;
            else
                _mazeMap[wallY][wallX] = INNER_WALL;
        }
    }

    // this chamber vertical walls generation
    if (isVerticalSplit) {
        splitX = GetRandomIntFromRange(startX + 1, endX - 1);
        for (int y = startY; y < endY + 1; y++) {
            int wallX = splitX;
            int wallY = y;

            // leave space if wall would be an obstacle for another wall's door
            if (_mazeMap[wallY - 1][wallX] == SPACE || _mazeMap[wallY + 1][wallX] == SPACE)
                _mazeMap[wallY][wallX] = RAW;
            else
                _mazeMap[wallY][wallX] = INNER_WALL;
        }
    }

    // generating spaces in walls
    if (isVerticalSplit && isHorizontalSplit) {
        // create a space in every wall but one
        int choice = GetRandomIntFromRange(0, 3);

        // top wall space
        if (choice != TOP_WALL_WITHOUT_SPACE) {
            int spaceX = splitX;
            int spaceY = GetRandomIntFromRange(startY, splitY - 1);
            _mazeMap[spaceY][spaceX] = SPACE;
        }

        // right wall space
        if (choice != RIGHT_WALL_WITHOUT_SPACE) {
            int spaceX = GetRandomIntFromRange(splitX + 1, endX);
            int spaceY = splitY;
            _mazeMap[spaceY][spaceX] = SPACE;
        }

        // bottom wall space
        if (choice != BOTTOM_WALL_WITHOUT_SPACE) {
            int spaceX = splitX;
            int spaceY = GetRandomIntFromRange(splitY + 1, endY);
            _mazeMap[spaceY][spaceX] = SPACE;
        }

        // left wall space
        if (choice != LEFT_WALL_WITHOUT_SPACE) {
            int spaceX = GetRandomIntFromRange(startX, splitX - 1);
            int spaceY = splitY;
            _mazeMap[spaceY][spaceX] = SPACE;
        }
    } else if (isHorizontalSplit) {
        // randomize space in the horizontal wall
        int spaceX = GetRandomIntFromRange(startX, endX);
        int spaceY = splitY;
        _mazeMap[spaceY][spaceX] = SPACE;
    } else if (isVerticalSplit) {
        // randomize space in the vertical wall
        int spaceX = splitX;
        int spaceY = GetRandomIntFromRange(startY, endY);
        _mazeMap[spaceY][spaceX] = SPACE;
    }

    // recursion branching
    if (isVerticalSplit && isHorizontalSplit) {
        // top-left chamber
        GenerateChamber(startX, startY, splitX - 1, splitY - 1);

        // top-right chamber
        GenerateChamber(splitX + 1, startY, endX, splitY - 1);

        // bottom-right chamber
        GenerateChamber(splitX + 1, splitY + 1, endX, endY);

        // top-right chamber
        GenerateChamber(startX, splitY + 1, splitX - 1, endY);
    } else if (isVerticalSplit) {
        // left chamber
        GenerateChamber(startX, startY, splitX - 1, endY);

        // right chamber
        GenerateChamber(splitX + 1, startY, endX, endY);
    } else {
        // top chamber
        GenerateChamber(startX, startY, endX, splitY - 1);

        // bottom chamber
        GenerateChamber(startX, splitY + 1, endX, endY);
    }
}


void Maze::GenerateMaze(int lengthX, int lengthY) {
    // initial empty maze generation
    for (int y = 0; y < lengthY; y++)
        for (int x = 0; x < lengthX; x++)
            _mazeMap[y][x] = (!x || !y || x == lengthX - 1 || y == lengthY - 1 ? OUTER_WALL : RAW);

    // _mazeMap _walls generation
    GenerateChamber(1, 1, lengthX - 2, lengthY - 2);

    // generating entry point
    int entryPoint = GetRandomIntFromRange(1, lengthX - 2);
    while (_mazeMap[1][entryPoint] == INNER_WALL)
        entryPoint = GetRandomIntFromRange(1, lengthX - 2);

    _mazeMap[0][entryPoint] = SPACE;

    // generating exit point
    int exitPoint = GetRandomIntFromRange(1, lengthX - 2);
    while (_mazeMap[lengthY - 2][exitPoint] == INNER_WALL)
        exitPoint = GetRandomIntFromRange(1, lengthX - 2);

    _mazeMap[lengthY - 1][exitPoint] = SPACE;

    // writing result into a file
    std::ofstream output("mazemap.txt");
    for (int y = 0; y < lengthY; y++) {
        for (int x = 0; x < lengthX; x++) {
            output << (_mazeMap[y][x] != SPACE && _mazeMap[y][x] != RAW ? '#' : ' ') << ' ';
        }
        output << '\n';
    }
}

