#include "camera.h"

void Camera::Init(sf::RenderWindow &window) {
  cam = window.getDefaultView();
  sf::Vector2f centre = GridToWorld(chunkSize / 2.0f, chunkSize / 2.0f);
  cam.setCenter(centre);
  window.setView(cam);
}

void Camera::Update(sf::RenderWindow &window) {
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    cam.move(0, -speed);
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    cam.move(0, speed);
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    cam.move(-speed, 0);
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    cam.move(speed, 0);
  window.setView(cam);
}

void Camera::Zoom(sf::RenderWindow &window, float delta) {
  float factor = (delta < 0) ? (1.0f + zoomSpeed) : (1.0f - zoomSpeed);
  float newZoom = zoomLevel * factor;

  if (newZoom < zoomMin || newZoom > zoomMax)
    return;

  zoomLevel = newZoom;
  cam.zoom(factor);
  window.setView(cam);
}

void Camera::ResizeView(sf::RenderWindow &window, const int gameWidth,
                        const int gameHeight) {
  cam.setSize(window.getSize().x, window.getSize().y);
  cam.zoom(zoomLevel); // reapply zoom after resize
  window.setView(cam);
}
