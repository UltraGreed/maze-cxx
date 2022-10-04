#ifndef THEBESTLABYRINTH_GENERATEMAZE_H
#define THEBESTLABYRINTH_GENERATEMAZE_H

#include <fstream>
#include <vector>
#include <random>

// vectors' concatenation
template<class T>
std::vector<T> operator+(std::vector<T> const &m, std::vector<T> const &n) {
    std::vector<T> v;
    v.reserve(m.size() + n.size());
    v.insert(v.end(), m.begin(), m.end());
    v.insert(v.end(), n.begin(), n.end());
    return v;
}

template<typename T>
std::vector<T> &operator+=(std::vector<T> &m, const std::vector<T> &n) {
    m.reserve(m.size() + n.size());
    m.insert(m.end(), n.begin(), n.end());
    return m;
}

// random generics initialization
std::random_device rd;
std::uniform_int_distribution<int> quadrupleDist(0, 3);
std::mt19937 gen(rd());

// small recursion just for fun
std::vector<std::vector<int>>
generateChamber(int xStart, int yStart, int xEnd, int yEnd, std::vector<std::vector<int>> prevSpaces) {
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
        // case we build both horizontal and vertical walls
        isVerticalSplit = true;
        isHorizontalSplit = true;
        ySplit = ySplitDist(gen);
        xSplit = xSplitDist(gen);
        // create a space in every wall but one
        int choice = quadrupleDist(gen);
        if (choice != 0) {
            // top wall space
            std::uniform_int_distribution<int> ySpaceDist(yStart, ySplit - 1);
            thisSpaces.push_back({xSplit, ySpaceDist(gen)});
        }
        if (choice != 1) {
            // right wall space
            std::uniform_int_distribution<int> xSpaceDist(xSplit + 1, xEnd);
            thisSpaces.push_back({xSpaceDist(gen), ySplit});
        }
        if (choice != 2) {
            // bottom wall space
            std::uniform_int_distribution<int> ySpaceDist(ySplit + 1, yEnd);
            thisSpaces.push_back({xSplit, ySpaceDist(gen)});
        }
        if (choice != 3) {
            // left wall space
            std::uniform_int_distribution<int> xSpaceDist(xStart, xSplit - 1);
            thisSpaces.push_back({xSpaceDist(gen), ySplit});
        }
    } else if (yEnd - yStart >= 2) {
        // case we build only a horizontal wall
        isHorizontalSplit = true;
        ySplit = ySplitDist(gen);
        // randomize wall space
        std::uniform_int_distribution<int> xSpaceDist(xStart, xEnd);
        thisSpaces.push_back(std::vector<int>{xSpaceDist(gen), ySplit});
    } else {
        // case we build only a vertical wall
        isVerticalSplit = true;
        xSplit = xSplitDist(gen);
        // randomize wall space
        std::uniform_int_distribution<int> ySpaceDist(yStart, yEnd);
        thisSpaces.push_back(std::vector<int>{xSplit, ySpaceDist(gen)});
    }

    // this chamber horizontal walls generation
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

    // this chamber vertical walls generation
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

    if (isVerticalSplit && isHorizontalSplit){
        // top-left chamber
        nextWalls += generateChamber(xStart, yStart, xSplit - 1, ySplit - 1, prevSpaces);
        // top-right chamber
        nextWalls += generateChamber(xSplit + 1, yStart, xEnd, ySplit - 1, prevSpaces);
        // bottom-right chamber
        nextWalls += generateChamber(xSplit + 1, ySplit + 1, xEnd, yEnd, prevSpaces);
        // top-right chamber
        nextWalls += generateChamber(xStart, ySplit + 1, xSplit - 1, yEnd, prevSpaces);
    } else if (isVerticalSplit){
        // left chamber
        nextWalls += generateChamber(xStart, yStart, xSplit - 1, yEnd, prevSpaces);
        // right chamber
        nextWalls += generateChamber(xSplit + 1, yStart, xEnd, yEnd, prevSpaces);
    } else {
        // top chamber
        nextWalls += generateChamber(xStart, yStart, xEnd, ySplit - 1, prevSpaces);
        // bottom chamber
        nextWalls += generateChamber(xStart, ySplit + 1, xEnd, yEnd, prevSpaces);
    }
    return nextWalls + thisWalls;
}


void generateMaze(int xLength, int yLength) {
    // initial empty maze generation
    char maze[yLength][xLength];
    for (int x = 0; x < xLength; x++)
        for (int y = 0; y < yLength; y++)
            maze[x][y] = (!x || !y || x == xLength - 1 || y == yLength - 1) ? '#' : '.';

    // maze walls generation
    auto mazeWalls = generateChamber(1, 1, xLength - 2, yLength - 2, {});

    // filling maze with previously generated walls
    for (std::vector<int> &wall: mazeWalls) {
        int x = wall.at(0);
        int y = wall.at(1);
        maze[x][y] = '#';
    }

    // generating random entry and exit points
    std::uniform_int_distribution<int> xEntryDist(1, xLength - 2);
    std::vector<int> entryPoint = {xEntryDist(gen), 0};
    while (maze[1][entryPoint.at(0)] == '#')
        entryPoint = {xEntryDist(gen), 0};
    maze[0][entryPoint.at(0)] = '.';
    std::vector<int> exitPoint = {xEntryDist(gen), yLength - 1};
    while (maze[yLength - 2][exitPoint.at(0)] == '#')
        exitPoint = {xEntryDist(gen), yLength - 1};
    maze[yLength - 1][exitPoint.at(0)] = '.';

    // writing result into a file
    std::ofstream output("output.txt");
    for (int x = 0; x < xLength; x++) {
        for (int y = 0; y < yLength; y++)
            output << maze[x][y] << ' ';
        output << '\n';
    }
}

#endif //THEBESTLABYRINTH_GENERATEMAZE_H
