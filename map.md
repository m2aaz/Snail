# Tile-Based Map System (SFML)

## Overview

This project implements a simple 2D tile-based map system using SFML. The map is built using a tilesheet and rendered through a grid-based system with procedural generation for object placement.

The system supports:
- Tilesheet-based rendering
- Coordinate mapping from grid → texture atlas
- Procedural object placement
- Layered rendering (ground + objects)
- Empty tile handling


## Tilesheet Structure

A single tilesheet image is used (`era1.png`) containing multiple terrain tiles such as grass, bushes, ferns, and rocks.

Each tile in the tilesheet is accessed using grid-based coordinates, which are converted into pixel coordinates using:

- `tileSpacing` (size of each tile in pixels)
- `tileLookup` (mapping of tile IDs to tilesheet positions)

Example: ```tileLookup[2] = {11, 11}; // Bush tile in tilesheet grid ```

This is later converted into pixel coordinates for rendering:
```cpp
pixelX = tilePos.x * tileSpacing;
pixelY = tilePos.y * tileSpacing;
```


## Map Structure

The world is represented using two 2D arrays:

### Ground Layer
- Always filled
- Represents base terrain (e.g., grass)

```cpp
vectorMapGround[y][x]
```

### Object Layer
- Contains optional decorations (bushes, rocks, ferns)
- Uses 0 to represent empty space

```cpp
vectorMapObject[y][x]
```


## Procedural Generation

Object placement is generated using a random number generator:

```cpp
// Generator: std::mt19937 rng;
if (tileNum < 75) return 0; // Empty
if (tileNum < 85) return 2; // Bush
if (tileNum < 95) return 3; // Fern
return 4;              // Rock
```

This ensures:
- Majority of tiles remain empty
- Natural distribution of objects across the map


## Tile Lookup System

Tile IDs are mapped to positions inside the tilesheet using an unordered map:

```cpp
tileLookup[1] = {4, 6};   // Grass
tileLookup[2] = {11, 11}; // Bush
tileLookup[3] = {15, 11}; // Fern
tileLookup[4] = {0, 13};  // Rock
```

This allows separation between:
- Logical tile IDs
- Physical tilesheet positions


## Rendering Pipeline

Rendering is done in two layers:

1. Ground Layer
Always rendered first:

```cpp
tileID = vectorMapGround[i][j];
tilePos = tileLookup[tileID];
TileToMap(tilePos, i, j);
window.draw(currentTile);
```

2. Object Layer
Rendered on top of ground layer:

```cpp
tileID = vectorMapObject[i][j];

if (tileID == 0)
    continue;

tilePos = tileLookup[tileID];
TileToMap(tilePos, i, j);
window.draw(currentTile);
```


## Coordinate System

The system uses grid-based indexing:
- i = row (Y-axis)
- j = column (X-axis)

Conversion to screen position:

```cpp
x = j * tileSpacing
y = i * tileSpacing
```

This ensures correct spatial alignment of tiles on screen.


## Tile Rendering Function

All tile rendering is handled through a single helper function:

```cpp
void TileToMap(sf::Vector2i tilePos, int i, int j) {
    int pX = tilePos.x * tileSpacing;
    int pY = tilePos.y * tileSpacing;

    currentTile.setTextureRect(sf::IntRect(
        pX, pY,
        tileSpacing, tileSpacing
    ));

    currentTile.setPosition(
        j * tileSpacing,
        i * tileSpacing
    );
}
```

This function:
- Extracts the correct tile from the tilesheet
- Positions it correctly in world space


## Errors To Look Out FOR:
- Confused tile IDs with actual tilesheet coordinates; fixed by introducing a proper lookup table (tileLookup) separating logic from rendering positions.
- Initially used a single map for everything (ground + objects), which prevented layering; fixed by splitting into vectorMapGround and vectorMapObject.
- Treated 0 inconsistently instead of defining it as “empty space”; standardized it so only object layer uses 0 to mean “nothing to render.”
- Swapped X and Y axes in rendering, causing distorted/half-screen output; fixed by using x = j * tileSpacing and y = i * tileSpacing.
- Performed tile lookup before checking for empty tiles, causing unnecessary work; fixed by checking tileID == 0 first.
- Misdiagnosed rendering issues as missing data instead of coordinate logic errors; fixed by correctly analyzing spatial mapping instead of generation.
- Mixed rendering logic and data handling too tightly; partially improved by introducing TileToMap() helper function for cleaner separation.