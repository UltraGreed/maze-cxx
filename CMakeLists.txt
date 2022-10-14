cmake_minimum_required(VERSION 3.23)
project(theBestLabyrinth)

set(CMAKE_CXX_STANDARD 14)

find_package(SFML 2.5 COMPONENTS graphics REQUIRED)

add_executable(theBestLabyrinth main.cpp GameHeaders/Player.h GameHeaders/Camera.h GameHeaders/config.h GameHeaders/GameState.h GameHeaders/Maze.h GameHeaders/Camera.cpp GameHeaders/Player.cpp GameHeaders/Gamestate.cpp GameHeaders/Maze.cpp)

target_link_libraries(theBestLabyrinth sfml-graphics)