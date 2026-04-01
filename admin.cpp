#include <SFML/Graphics.hpp> 
#include "tools.cpp"
#include "chunks.cpp"
#include <iostream>

struct Camera {
    public:
        sf::View cam;
        float speed=5.0f;

    public:
        void Init(sf::RenderWindow& window) {
            cam = window.getDefaultView();
            window.setView(cam);
        }

        void Update(sf::RenderWindow& window) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
                cam.move(0, -speed);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
                cam.move(0, speed);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
                cam.move(-speed, 0);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
                cam.move(speed, 0);

            window.setView(cam);
        }

        void ResizeView(sf::RenderWindow& window, const int gameWidth, const int gameHeight) {
            float aspectRatio = float(window.getSize().x)/float(window.getSize().y);
            cam.setSize(gameWidth * aspectRatio, gameHeight);
        }
};

class Admin {
    private:
        const int gameWidth = 1280;
        const int gameHeight = 720;

    public:
        sf::RenderWindow window;
        Clock delta;
        sf::Event event;
        Chunk newChunk;
        ChunkRenderer Renderer;
        Camera cam;


    private:
        void Update() {
            cam.Update(window);
        }
        void Draw() {
            window.clear(sf::Color(30, 30, 30));
            Renderer.RenderChunk(window, newChunk);
            delta.displayFPS(window);
            window.display();
        }

    public:
        void Init() {
        
            // Initialise Window.
            window.create(sf::VideoMode(gameWidth, gameHeight), "The Immortal Snail");

            // Initialise Chunk Renderer & Generate Chunk.
            Renderer.Init();
            GenerateChunk(newChunk);

            // Set Window Frames.
            delta.Init();
            delta.setFrameRate(window, 60);

            // Initialise Camera
            cam.Init(window);
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
                if (event.type == sf::Event::Resized) 
                {cam.ResizeView(window, gameWidth, gameHeight);}
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
