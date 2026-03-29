#include <SFML/Graphics.hpp> 
#include "tools.cpp"
#include <iostream>

class Admin {
    private:
        const int gameWidth = 1280;
        const int gameHeight = 720;

    public:
        sf::RenderWindow window;
        Clock delta;
        sf::Event event;

    private:
        void Update() {}
        void Draw() {
            window.clear(sf::Color::Black);
            delta.displayFPS(window);
            window.display();
        }

    public:
        void Init() {
        
            // Initialise Window
            window.create(sf::VideoMode(gameWidth, gameHeight), "The Immortal Snail");

            // Set Window Frames.
            delta.Init();
            delta.setFrameRate(window, 60);
        }

        void Run() {
            while (window.isOpen()) {
                delta.cycleStart();
                Exit();
                Update();
                Draw();
            }
        }

        void Exit() {
            // Handle Event (Closing)
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) 
                {window.close(); std::cout << "Window Closed." << std::endl;}
            }
        }

        void Save() {}
};

int main() {
    Admin gameOperator;
    gameOperator.Init();
    gameOperator.Run();
}
