cmake_minimum_required(VERSION 3.23)
project(theBestLabyrinth)

set(CMAKE_CXX_STANDARD 14)

add_executable(theBestLabyrinth generateMaze.h main.cpp keyRead.h Player.h Camera.h)
