
#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <vector>

static constexpr int chunkSize = 16;
std::mt19937 rng(std::random_device{}());
std::uniform_int_distribution<int> dist(1, 4);

class Chunk {
    public: 
        std::vector<std::vector<int>> vectorMapGround;
        std::vector<std::vector<int>> vectorMapObject;
        sf::Vector2i chunkCoordinates;

        Chunk() {
            vectorMapGround.resize(chunkSize, std::vector<int>(chunkSize, 0));
            vectorMapObject.resize(chunkSize, std::vector<int>(chunkSize, 0));
        }
};


// 0: Grass, 1: Anything else.
void GenerateChunk(Chunk& chunk) {
    for (int i=0; i<chunkSize; i++) {
        for (int j=0; j<chunkSize; j++) {
            chunk.vectorMapGround[i][j] = 0;
            chunk.vectorMapObject[i][j] = dist(rng);
        }
    }
}

void LoadChunk(const Chunk& chunk) {
    
}

void DisplayChunk(const Chunk& chunk) {
    for (int i=0; i<chunkSize; i++) {
        for (int j=0; j<chunkSize; j++) {
            std::cout << chunk.vectorMapGround[i][j] << ", ";
        }
        std::cout << std::endl;
    }

    std::cout << std::endl; 

    for (int i=0; i<chunkSize; i++) {
        for (int j=0; j<chunkSize; j++) {
            std::cout << chunk.vectorMapObject[i][j] << ", ";
        }
        std::cout << std::endl;
    }
}

int main() {
    // sf::RenderWindow window;
    // sf::Event event;
    // window.create(sf::VideoMode(1280, 720), "The Immortal Snail");
    // while (window.isOpen()) {
    //     while (window.pollEvent(event)) {
    //         if (event.type == sf::Event::Closed) {
    //             window.close();
    //         }
    //     }
    //     window.clear(sf::Color(30, 30, 30));
    //     window.display();
    // }
    Chunk chunk;
    GenerateChunk(chunk);
    DisplayChunk(chunk);

}