#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <random>

int fetchEra() {return 1;}

struct MapData {
    std::vector<std::vector<int>> vectorMapGround;
    std::vector<std::vector<int>> vectorMapObject;
};

struct EraConfig {
    std::string fileloc;
    int numItems;

    EraConfig(std::string fileloc = "", int numItems = 0) : fileloc(fileloc), numItems(numItems) {}
};

class MapConfig {
public:
    int mapHeight = 720;
    int mapWidth = 1280;
    int tileSpacing = 16;

    std::unordered_map<int, EraConfig> eraLookup;
    std::unordered_map<int, sf::Vector2i> tileLookup;

    MapConfig() {
        // Era sprite sheets
        eraLookup[1] = EraConfig("tilemap/era1.png", 4);
        eraLookup[2] = EraConfig("tilemap/era2.png");
        eraLookup[3] = EraConfig("tilemap/era3.png");
        eraLookup[4] = EraConfig("tilemap/era4.png");
        eraLookup[5] = EraConfig("tilemap/era5.png");
        eraLookup[6] = EraConfig("tilemap/era6.png");
        eraLookup[7] = EraConfig("tilemap/era7.png");

        // Tile ID -> spritesheet grid position (col, row)
        tileLookup[1] = {4, 6};
        tileLookup[2] = {11, 11};
        tileLookup[3] = {15, 11};
        tileLookup[4] = {0, 13};
    }

    EraConfig getEra(int id) const {
        return eraLookup.at(id);
    }

    int getMapRows() const { return mapHeight / tileSpacing; }
    int getMapCols() const { return mapWidth  / tileSpacing; }
};


class WorldGen {
private:
    MapConfig config;
    MapData map;

    std::mt19937 rng;
    std::uniform_int_distribution<int> dist;

public:
    WorldGen() : rng(std::random_device{}()), dist(0, 99) {
        int rows = config.getMapRows();
        int cols = config.getMapCols();

        map.vectorMapGround.assign(rows, std::vector<int>(cols, 0));
        map.vectorMapObject.assign(rows, std::vector<int>(cols, 0));

        GenerateWorld();
    }

    // Returns a random object tile ID using weighted probability.
    int randomTile() {
        int roll = dist(rng);
        if (roll < 75) return 0; 
        if (roll < 85) return 2; 
        if (roll < 95) return 3;
        return 4;                
    }

    void GenerateWorld() {
        int rows = config.getMapRows();
        int cols = config.getMapCols();

        for (int i=0; i<rows; i++) {
            for (int j=0; j<cols; j++) {
                map.vectorMapGround[i][j] = 1;        
                map.vectorMapObject[i][j] = randomTile();
            }
        }
    }

    MapData& getMap() { return map; }
};


class RenderMap {
private:
    MapConfig config;
    MapData map;

    sf::Texture texture;
    sf::Sprite currentTile;
    std::string fileloc;

public:
    RenderMap(const MapData& inputMap) : map(inputMap) {Load();}

    void Load() {
        int era = fetchEra();
        fileloc = config.getEra(era).fileloc;

        if (texture.loadFromFile(fileloc)) {
            currentTile.setTexture(texture);
            std::cout << "Loaded TileMap: " << fileloc << std::endl;
        } else {
            std::cout << "Failed to load TileMap: " << fileloc << std::endl;
        }
    }

    void TileToMap(sf::Vector2i tilePos, int row, int col) {
        int ts = config.tileSpacing;
        currentTile.setTextureRect(sf::IntRect(
            tilePos.x * ts,
            tilePos.y * ts,
            ts, ts
        ));

        currentTile.setPosition(
            static_cast<float>(col * ts),
            static_cast<float>(row * ts)
        );
    }

    void Draw(sf::RenderWindow& window) {
        int rows = config.getMapRows();
        int cols = config.getMapCols();

        for (int i=0; i<rows; i++) {
            for (int j=0; j<cols; j++) {

                int groundID = map.vectorMapGround[i][j];
                TileToMap(config.tileLookup.at(groundID), i, j);
                window.draw(currentTile);

                int objectID = map.vectorMapObject[i][j];
                if (objectID == 0) continue;

                TileToMap(config.tileLookup.at(objectID), i, j);
                window.draw(currentTile);
            }
        }
    }
};

int main() {
}