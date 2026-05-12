#include "game_operator.h"
#include "enemy.h"
#include <iostream>

void GameOperator::Update() { 
	player.Update();
	sf::Vector2i playerGrid = player.GetGridPosition();

	// Spawn enemies on a fixed interval, always near the player but not too far.
	enemySpawnTimer += delta.deltaTime / 1000.0f;
	if (enemySpawnTimer >= Enemy::spawnIntervalSeconds) {
		enemySpawnTimer -= Enemy::spawnIntervalSeconds;
		Enemy spawned;
		if (Enemy::TrySpawnNearPlayer(playerGrid, cMap, spawned)) {
			enemies.push_back(std::move(spawned));
			std::cout << "Auto-spawned enemy at " << spawned.GetGridPosition().x << "," << spawned.GetGridPosition().y << std::endl;
		} else {
			std::cout << "Auto-spawn failed: no valid spawn found" << std::endl;
		}
	}

	sf::FloatRect playerBounds = player.GetGlobalBounds();
	const int playerDamage = 10;

	for (auto it = enemies.begin(); it != enemies.end();) {
		it->Update(delta.deltaTime, playerGrid);
		if (playerBounds.intersects(it->GetGlobalBounds())) {
			it->TakeDamage(playerDamage);
			std::cout << "Player hit enemy, enemy hp=" << it->GetHP() << std::endl;
			if (it->IsDead()) {
				it = enemies.erase(it);
				continue;
			}
		}
		++it;
	}

	sf::Vector2f playerWorld = GridToWorld((float)playerGrid.y, (float)playerGrid.x);
	cam.FollowPlayer(window, playerWorld);
 }

void GameOperator::Draw() {
	window.clear(sf::Color(30, 30, 30));
	Renderer.RenderChunkMap(window, cMap);

	for (auto &enemy : enemies) {
		enemy.Draw(window);
	}

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
			bool shiftHeld = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift);
			if (shiftHeld) {
				sf::Vector2i clickPixel(event.mouseButton.x, event.mouseButton.y);
				sf::Vector2f clickWorld = window.mapPixelToCoords(clickPixel);
				sf::Vector2i clickGrid = WorldToGrid(clickWorld);

				if (event.mouseButton.button == sf::Mouse::Left) {
					Enemy spawned;
					if (Enemy::TrySpawnAtGrid(clickGrid, cMap, spawned)) {
						enemies.push_back(std::move(spawned));
						std::cout << "Spawned enemy at " << clickGrid.x << "," << clickGrid.y << std::endl;
					}
				}
				if (event.mouseButton.button == sf::Mouse::Right) {
					sf::Vector2i playerGrid = player.GetGridPosition();
					Enemy spawned;
					if (Enemy::TrySpawnNearPlayer(playerGrid, cMap, spawned)) {
						enemies.push_back(std::move(spawned));
						std::cout << "Spawned enemy near player at " << spawned.GetGridPosition().x << "," << spawned.GetGridPosition().y << std::endl;
					}
				}
			} else {
				if (event.mouseButton.button == sf::Mouse::Left) {
					// behave.placeChunk(window, cMap);
				}
				if (event.mouseButton.button == sf::Mouse::Right) {
					// behave.deleteChunk(window, cMap);
				}
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
