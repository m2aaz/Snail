#pragma once
#include <SFML/Graphics.hpp>
#include "chunks.cpp"

struct Camera {
    public:
        sf::View cam;
        float speed=20.0f;
        int tileSpacing = 32;
        int tileWidth = 32;
        int tileHeight = 32;
        int chunkSize = 16;

    public:
        void Init(sf::RenderWindow& window) {
            cam = window.getDefaultView();
            float isoWidth  = chunkSize * (tileWidth / 2.0f); 
            float isoHeight = chunkSize * (tileHeight / 2.0f);
            cam.setCenter(isoWidth/(tileWidth/2), isoHeight); 
            window.setView(cam);
        }

        void Update(sf::RenderWindow& window) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
                cam.move(0, -speed);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
                cam.move(0, speed);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
                cam.move(-speed, 0);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
                cam.move(speed, 0);

            window.setView(cam);
        }

        void ResizeView(sf::RenderWindow& window, const int gameWidth, const int gameHeight) {
            float aspectRatio = float(window.getSize().x)/float(window.getSize().y);
            cam.setSize(gameWidth * aspectRatio, gameHeight);
        }
};


class Clock {
    private:
        sf::Clock clock;
        sf::Time deltaTimer;
        sf::Text dispFPS;
        sf::Font dispFPSFont;

    public:
        float deltaTime;
        float fps=60.f;
        bool display=false;

    public:
        void Init() {
            if (display) {
                dispFPSFont.loadFromFile("fonts/Arial.ttf");
                dispFPS.setFont(dispFPSFont);
                dispFPS.setScale(sf::Vector2f(0.5f, 0.5f));
            }
        }
        void cycleStart() {
            deltaTimer = clock.restart();
            deltaTime = deltaTimer.asMilliseconds();
        }

        void setFrameRate(sf::RenderWindow& window, int fps) {
            window.setFramerateLimit(fps);
        }

        void displayFPS(sf::RenderWindow& window) {
            if (display) {
                // Static cast to int to avoid trailing 0's with float->string conversion
                dispFPS.setString("FPS: " + std::to_string(int(round(1000/deltaTime))));
                window.draw(dispFPS);
            }
        }
};
