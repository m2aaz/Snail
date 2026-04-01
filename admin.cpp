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
        Chunk newChunk;

        Chunk newChunk1;
        Chunk newChunk2;
        Chunk newChunk3;
        Chunk newChunk4;

        ChunkRenderer Renderer;
        Camera cam;


    private:
        void Update() {
            cam.Update(window);
        }
        void Draw() {
            window.clear(sf::Color(30, 30, 30));
            Renderer.RenderChunk(window, newChunk);
            Renderer.RenderChunk(window, newChunk1);
            Renderer.RenderChunk(window, newChunk2);
            Renderer.RenderChunk(window, newChunk3);
            Renderer.RenderChunk(window, newChunk4);

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

            GenerateChunk(newChunk1);
            GenerateChunk(newChunk2);
            GenerateChunk(newChunk3);
            GenerateChunk(newChunk4);

            newChunk1.setCoordinates(-1, 0);
            newChunk2.setCoordinates(0, -1);
            newChunk3.setCoordinates(0, 1);
            newChunk4.setCoordinates(1, 0);


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
    GameOperator admin;
    admin.Init();
    admin.Run();
}
