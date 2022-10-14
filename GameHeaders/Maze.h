//
// Created by ultragreed on 10/10/22.
//

#pragma once

#include <vector>
#include <random>

template<class T>
std::vector<T> operator+(std::vector<T> const &m, std::vector<T> const &n) {
    std::vector<T> v;
    v.reserve(m.size() + n.size());
    v.insert(v.end(), m.begin(), m.end());
    v.insert(v.end(), n.begin(), n.end());
    return v;
}

template<class T>
std::vector<T> &operator+=(std::vector<T> &m, const std::vector<T> &n) {
    m.reserve(m.size() + n.size());
    m.insert(m.end(), n.begin(), n.end());
    return m;
}

class Maze {
public:
    Maze(int mazeWidth, int mazeHeight);

    std::vector<std::vector<int>> GetWalls();

private:
    std::random_device _rd;
    std::mt19937 *_gen;
    std::uniform_int_distribution<int> *_quadrupleDist;
    std::vector<std::vector<int>> _walls;

    std::vector<std::vector<int>>
    GenerateChamber(int xStart, int yStart, int xEnd, int yEnd, std::vector<std::vector<int>> prevSpaces);

    std::vector<std::vector<int>> GenerateMaze(int xLength, int yLength);
};