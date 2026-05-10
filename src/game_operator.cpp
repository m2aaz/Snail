#include "game_operator.h"
#include <iostream>

void GameOperator::Update() { 
	player.Update();
	sf::Vector2i playerGrid = player.GetGridPosition();
	sf::Vector2f playerWorld = GridToWorld((float)playerGrid.y, (float)playerGrid.x);
	cam.FollowPlayer(window, playerWorld);
 }

void GameOperator::Draw() {
	window.clear(sf::Color(30, 30, 30));
	Renderer.RenderChunkMap(window, cMap);

	delta.displayFPS(window);
	player.Draw(window);
	window.display();
}

void GameOperator::Init() {
	// Initialise Window.
	window.create(sf::VideoMode(gameWidth, gameHeight), "The Immortal Snail");

	// Initialise Chunk Renderer & Generate Initial Chunks.
	Renderer.Init();
	
	// Spawn chunks in a large grid around origin to create a huge world with borders
	for (int x = -10; x <= 10; x++) {
		for (int y = -10; y <= 10; y++) {
			spawnChunk(sf::Vector2i(x, y), cMap);
		}
	}

	// Place player on a non-water tile in the origin chunk.
	if (cMap.Exists({0, 0})) {
		Chunk& originChunk = cMap.Get({0, 0});
		for (int i = 0; i < chunkSize; i++) {
			for (int j = 0; j < chunkSize; j++) {
				if (originChunk.vectorMapGround[i][j] != 0) {
					player.SetGridPosition({j, i});
					goto player_spawned;
				}
			}
		}
	}
player_spawned:

	// Set Window Frames.
	delta.Init();
	delta.setFrameRate(window, 120);

	// Initialise Camera
	cam.Init(window);

	// Initialise Player
	player.Init();
}

void GameOperator::Run() {
	while (window.isOpen()) {
		delta.cycleStart();
		Exit();
		Update();
		Draw();
	}
}

void GameOperator::Exit() {
	// Handle Event (Closing)
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Resized) {
			cam.ResizeView(window, gameWidth, gameHeight);
		}
		if (event.type == sf::Event::MouseButtonPressed) {
			if (event.mouseButton.button == sf::Mouse::Left) {
				behave.placeChunk(window, cMap);
			}
			if (event.mouseButton.button == sf::Mouse::Right) {
				behave.deleteChunk(window, cMap);
			}
		}
		if (event.type == sf::Event::KeyPressed) {
			player.HandleInput(event, cMap);
			if (event.key.code == sf::Keyboard::F) {
				delta.toggleFPS();
			}
			if (event.key.code == sf::Keyboard::C) {
				cam.Zoom(window, 1);
			}
			if (event.key.code == sf::Keyboard::X) {
				cam.Zoom(window, -1);
			}
		}
		if (event.type == sf::Event::Closed) {
			window.close();
			std::cout << "Window Closed." << std::endl;
		}
	}
}

void GameOperator::Save() {}
