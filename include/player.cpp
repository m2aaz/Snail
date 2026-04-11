
#include <SFML/Graphics.hpp>
#include <iostream>
#include "common.h"


class Player {
    private:
        sf::Texture texture;
        sf::Sprite playerSprite;
        float speed=5.0f;
        std::string fileloc = "sprites/player.png";

    public:
        void Init() {
            if (texture.loadFromFile(fileloc)) {
                playerSprite.setTexture(texture);
                playerSprite.setPosition(0, 0);
                playerSprite.setTextureRect({0, 0, tileSpacing*2, tileSpacing*2});
            }
        }
        void Move() {
            // Moving in screen space right now, only for testing, will convert to grid
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                playerSprite.move(0, -speed);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                playerSprite.move(0, speed);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                playerSprite.move(-speed, 0);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                playerSprite.move(speed, 0);
        }
        void Update() {
            Move();
        }
        void Draw(sf::RenderWindow& window) {
            window.draw(playerSprite);
        }
};