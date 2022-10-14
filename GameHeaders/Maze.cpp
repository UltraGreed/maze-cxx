//
// Created by ultragreed on 10/14/22.
//
#include <fstream>
#include "Maze.h"
#include <vector>
#include <random>

Maze::Maze(int mazeWidth, int mazeHeight) {
    _gen = new std::mt19937(_rd());
    _quadrupleDist = new std::uniform_int_distribution<int>(0, 3);
    _walls = GenerateMaze(mazeWidth, mazeHeight);
}

std::vector<std::vector<int>> Maze::GetWalls() {
    return _walls;
}

std::vector<std::vector<int>>
Maze::GenerateChamber(int xStart, int yStart, int xEnd, int yEnd, std::vector<std::vector<int>> prevSpaces) {
    if (xEnd - xStart == 0 || yEnd - yStart == 0 || xEnd - xStart <= 1 && yEnd - yStart <= 1)
        // exit if no space left
        return {};

    // possible split coordinates distributions
    std::uniform_int_distribution<int> ySplitDist(yStart + 1, yEnd - 1);
    int ySplit;

    std::uniform_int_distribution<int> xSplitDist(xStart + 1, xEnd - 1);
    int xSplit;

    // variable containing this and further recursion results
    std::vector<std::vector<int>> thisWalls;
    std::vector<std::vector<int>> nextWalls;

    bool isVerticalSplit, isHorizontalSplit;
    std::vector<std::vector<int>> thisSpaces;

    if (xEnd - xStart >= 2 && yEnd - yStart >= 2) {
        // case we build both horizontal and vertical _walls
        isVerticalSplit = true;
        isHorizontalSplit = true;
        ySplit = ySplitDist(*_gen);
        xSplit = xSplitDist(*_gen);
        // create a space in every wall but one
        int choice = (*_quadrupleDist)(*_gen);
        if (choice != 0) {
            // top wall space
            std::uniform_int_distribution<int> ySpaceDist(yStart, ySplit - 1);
            thisSpaces.push_back({xSplit, ySpaceDist(*_gen)});
        }
        if (choice != 1) {
            // right wall space
            std::uniform_int_distribution<int> xSpaceDist(xSplit + 1, xEnd);
            thisSpaces.push_back({xSpaceDist(*_gen), ySplit});
        }
        if (choice != 2) {
            // bottom wall space
            std::uniform_int_distribution<int> ySpaceDist(ySplit + 1, yEnd);
            thisSpaces.push_back({xSplit, ySpaceDist(*_gen)});
        }
        if (choice != 3) {
            // left wall space
            std::uniform_int_distribution<int> xSpaceDist(xStart, xSplit - 1);
            thisSpaces.push_back({xSpaceDist(*_gen), ySplit});
        }
    } else if (yEnd - yStart >= 2) {
        // case we build only a horizontal wall
        isHorizontalSplit = true;
        ySplit = ySplitDist(*_gen);
        // randomize wall space
        std::uniform_int_distribution<int> xSpaceDist(xStart, xEnd);
        thisSpaces.push_back(std::vector<int>{xSpaceDist(*_gen), ySplit});
    } else {
        // case we build only a vertical wall
        isVerticalSplit = true;
        xSplit = xSplitDist(*_gen);
        // randomize wall space
        std::uniform_int_distribution<int> ySpaceDist(yStart, yEnd);
        thisSpaces.push_back(std::vector<int>{xSplit, ySpaceDist(*_gen)});
    }

    // this chamber horizontal _walls generation
    if (isHorizontalSplit) {
        for (int i = xStart; i < xEnd + 1; i++) {
            // check if here should be left a space
            bool isSpace = false;
            for (std::vector<int> &space: thisSpaces) {
                if (space == std::vector<int>{i, ySplit}) {
                    isSpace = true;
                    break;
                }
            }
            if (isSpace) continue;
            // check if wall would be an obstacle for another wall's door
            bool isObstacle = false;
            for (std::vector<int> &space: prevSpaces) {
                if (space.at(1) == ySplit && (space.at(0) - 1 == i || space.at(0) + 1 == i)) {
                    isObstacle = true;
                    break;
                }
            }
            if (isObstacle) continue;
            thisWalls.push_back(std::vector<int>{i, ySplit});
        }
    }

    // this chamber vertical _walls generation
    if (isVerticalSplit) {
        for (int i = yStart; i < yEnd + 1; i++) {
            // check if here should be left a space
            bool isSpace = false;
            for (std::vector<int> &space: thisSpaces) {
                if (space == std::vector<int>{xSplit, i}) {
                    isSpace = true;
                    break;
                }
            }
            if (isSpace) continue;
            // check if wall would be an obstacle for another wall's door
            bool isObstacle = false;
            for (std::vector<int> &thisSpace: prevSpaces) {
                if (thisSpace.at(0) == xSplit && (thisSpace.at(1) - 1 == i || thisSpace.at(1) + 1 == i)) {
                    isObstacle = true;
                    break;
                }
            }
            if (isObstacle) continue;
            thisWalls.push_back(std::vector<int>{xSplit, i});
        }
    }

    prevSpaces += thisSpaces;

    if (isVerticalSplit && isHorizontalSplit) {
        // top-left chamber
        nextWalls += GenerateChamber(xStart, yStart, xSplit - 1, ySplit - 1, prevSpaces);
        // top-right chamber
        nextWalls += GenerateChamber(xSplit + 1, yStart, xEnd, ySplit - 1, prevSpaces);
        // bottom-right chamber
        nextWalls += GenerateChamber(xSplit + 1, ySplit + 1, xEnd, yEnd, prevSpaces);
        // top-right chamber
        nextWalls += GenerateChamber(xStart, ySplit + 1, xSplit - 1, yEnd, prevSpaces);
    } else if (isVerticalSplit) {
        // left chamber
        nextWalls += GenerateChamber(xStart, yStart, xSplit - 1, yEnd, prevSpaces);
        // right chamber
        nextWalls += GenerateChamber(xSplit + 1, yStart, xEnd, yEnd, prevSpaces);
    } else {
        // top chamber
        nextWalls += GenerateChamber(xStart, yStart, xEnd, ySplit - 1, prevSpaces);
        // bottom chamber
        nextWalls += GenerateChamber(xStart, ySplit + 1, xEnd, yEnd, prevSpaces);
    }
    return nextWalls + thisWalls;
}


std::vector<std::vector<int>>
Maze::GenerateMaze(int xLength, int yLength) {
    // initial empty maze generation
    char maze[yLength][xLength];
    for (int x = 0; x < xLength; x++)
        for (int y = 0; y < yLength; y++)
            maze[x][y] = (!x || !y || x == xLength - 1 || y == yLength - 1) ? '#' : ' ';

    // maze _walls generation
    auto mazeWalls = GenerateChamber(1, 1, xLength - 2, yLength - 2, {});

    // filling maze with previously Generated _walls
    for (std::vector<int> &wall: mazeWalls) {
        int x = wall.at(0);
        int y = wall.at(1);
        maze[x][y] = '#';
    }

    // generating random entry and exit points
    std::uniform_int_distribution<int> xEntryDist(1, xLength - 2);
    std::vector<int> entryPoint = {xEntryDist(*_gen), 0};
    while (maze[1][entryPoint.at(0)] == '#')
        entryPoint = {xEntryDist(*_gen), 0};
    maze[0][entryPoint.at(0)] = ' ';
    std::vector<int> exitPoint = {xEntryDist(*_gen), yLength - 1};
    while (maze[yLength - 2][exitPoint.at(0)] == '#')
        exitPoint = {xEntryDist(*_gen), yLength - 1};
    maze[yLength - 1][exitPoint.at(0)] = ' ';

    for (int x = 0; x < xLength; x++)
        for (int y = 0; y < yLength; y++)
            if ((!x || !y || x == xLength - 1 || y == yLength - 1) &&
                std::vector<int>{x, y} != entryPoint &&
                std::vector<int>{x, y} != exitPoint
                    ) {
                mazeWalls.push_back(std::vector<int>{x, y});
            }

    // writing result into a file
    std::ofstream output("output.txt");
    for (int x = 0; x < xLength; x++) {
        for (int y = 0; y < yLength; y++) {
            output << maze[x][y] << ' ';
        }
        output << '\n';
    }
    return mazeWalls;
}

