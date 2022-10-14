#include "cstdio"
#include "GameHeaders/GameState.h"
#include "GameHeaders/config.h"
#include <SFML/Graphics.hpp>

using namespace config;

int main() {
    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight),
                            "The Best Labyrinth",
                            sf::Style::Default
    );
    GameState gameState(window);
    window.setKeyRepeatEnabled(false);

    bool isWorking = true;
    while (isWorking) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                isWorking = false;
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    isWorking = false;
                    window.close();
                } else if (event.key.code == sf::Keyboard::W)
                    gameState.HandleInput('w'); // move forward
                else if (event.key.code == sf::Keyboard::D)
                    gameState.HandleInput('d'); // move right
                else if (event.key.code == sf::Keyboard::S)
                    gameState.HandleInput('s'); // move back
                else if (event.key.code == sf::Keyboard::A)
                    gameState.HandleInput('a'); // move left
                else if (event.key.code == sf::Keyboard::E)
                    gameState.HandleInput('e'); // rotate right
                else if (event.key.code == sf::Keyboard::Q)
                    gameState.HandleInput('q'); // rotate left
                else if (event.key.code == sf::Keyboard::Space) // debug
                    gameState.HandleInput('X');
                else
                    printf("Unhandled key input: %d\n", event.key.code);
            }
        }


        window.clear(sf::Color::Blue);
        gameState.Update();
        window.display();
    }

    return 0;
}


