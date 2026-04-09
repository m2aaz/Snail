#pragma once
#include "common.h"

struct Camera {
public:
  sf::View cam;
  float speed = 20.0f;
  float zoomLevel = 1.0f;
  float zoomSpeed = 0.1f;
  float zoomMin = 0.25f;
  float zoomMax = 4.0f;
  int chunkSize = 16;

public:
  void Init(sf::RenderWindow &window);
  void Update(sf::RenderWindow &window);
  void Zoom(sf::RenderWindow &window, float delta);
  void ResizeView(sf::RenderWindow &window, const int gameWidth,
                  const int gameHeight);
};
