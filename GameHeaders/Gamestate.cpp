//
// Created by ultragreed on 10/14/22.
//

#include "Player.h"
#include "Camera.h"
#include "Maze.h"
#include "config.h"
#include "GameState.h"

using namespace config;

GameState::GameState(sf::RenderWindow &window) {
    _player = new Player(playerSpeed, playerAngularSpeed);
    _maze = new Maze(mazeWidth, mazeHeight);
    _camera = new Camera(screenWidth, screenHeight, _maze->GetWalls());
    _window = &window;
}

void GameState::Update() {
    sf::Time deltaTime = _clock.restart();
    _player->Update(deltaTime.asSeconds());
    _camera->Draw(_player->GetX(), _player->GetY(), _player->GetAngle(), *_window);
}

void GameState::HandleInput(char input) {
    if (input == 'X')
        _player->Debug();
    else
        _player->PutMoveInQueue(input);
}


