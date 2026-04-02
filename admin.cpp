#include <SFML/Graphics.hpp> 
#include "tools.cpp"
#include "chunks.cpp"
#include <iostream>

class GameOperator {
    private:
        const int gameWidth = 1280;
        const int gameHeight = 720;

    public:
        sf::RenderWindow window;
        Clock delta;
        sf::Event event;
        ChunkRenderer Renderer;
        Camera cam;
        ChunkMap cMap;


    private:
        void Update() {
            cam.Update(window);
        }
        void Draw() {
            window.clear(sf::Color(30, 30, 30));
            Renderer.RenderChunkMap(window, cMap);

            delta.displayFPS(window);
            window.display();
        }

    public:
        void Init() {
        
            // Initialise Window.
            window.create(sf::VideoMode(gameWidth, gameHeight), "The Immortal Snail");

            // Initialise Chunk Renderer & Generate Chunk.
            Renderer.Init();
            spawnChunk(-1, 0, cMap);
            spawnChunk(0, -1, cMap);
            spawnChunk(0, 1, cMap);
            spawnChunk(1, 0, cMap);
            spawnChunk(0, 0, cMap);

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
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        placeChunk(window, cMap);
                    }
                    if (event.mouseButton.button == sf::Mouse::Right) {
                        deleteChunk(window, cMap);
                    }
                }
                if (event.type == sf::Event::Closed) 
                {window.close(); std::cout << "Window Closed." << std::endl;}
            }
        }

        void Save() {}
};

int main() {
    GameOperator admin;
    admin.Init();
    admin.Run();
}
