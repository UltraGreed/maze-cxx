//
// Created by ultragreed on 10/14/22.
//

#include "Player.h"
#include "Camera.h"
#include "Maze.h"
#include "config.h"

#include "GameState.h"

GameState::GameState() {
    _maze = new Maze(config::mazeWidth, config::mazeHeight);
    _player = new Player(config::playerSpeed, config::playerAngularSpeed, _maze->GetMazeMap(), config::mazeWidth,
                         config::mazeHeight, config::playerX, config::playerY, config::playerAngle);
    _camera = new Camera(config::screenWidth, config::screenHeight, _maze->GetMazeMap(), config::mazeWidth,
                         config::mazeHeight, config::cameraHeightMultiplier, config::fieldOfView,
                         config::maxRange, config::brightness);

    _window = new sf::RenderWindow(sf::VideoMode(config::screenWidth, config::screenHeight),
                                   "The Best Labyrinth",
                                   sf::Style::Default
    );
    _window->setKeyRepeatEnabled(false);
}

bool GameState::Update() {
    bool isWorking = HandleEvents();

    sf::Time deltaTime = _clock.restart();

    _player->Update(deltaTime.asSeconds());

    _window->clear(sf::Color(config::bgRGB, config::bgRGB, config::bgRGB));

    _camera->Draw(_player->GetX(), _player->GetY(), _player->GetAngle(), *_window);

    _window->display();

    return isWorking;
}


void GameState::HandleInput(char input) {
    if (input == 'X')
        _player->Debug();
    else
        _player->PutMoveInQueue(input);
}

bool GameState::HandleEvents() {
    sf::Event event{};
    while (_window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            _window->close();
            return false;
        }
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) {
                _window->close();
                return false;
            } else if (event.key.code == sf::Keyboard::W)
                HandleInput('w'); // move forward
            else if (event.key.code == sf::Keyboard::D)
                HandleInput('d'); // move right
            else if (event.key.code == sf::Keyboard::S)
                HandleInput('s'); // move back
            else if (event.key.code == sf::Keyboard::A)
                HandleInput('a'); // move left
            else if (event.key.code == sf::Keyboard::E)
                HandleInput('e'); // rotate right
            else if (event.key.code == sf::Keyboard::Q)
                HandleInput('q'); // rotate left
            else if (event.key.code == sf::Keyboard::Space) // debug
                HandleInput('X');
            else
                printf("Unhandled key input: %d\n", event.key.code);
        }
    }
    return true;
}
