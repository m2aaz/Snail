#include "camera.h"
#include <cmath>

void Camera::Init(sf::RenderWindow &window) {
	cam = window.getDefaultView();
	sf::Vector2f centre = GridToWorld(chunkSize / 2.0f, chunkSize / 2.0f);
	cam.setCenter(centre);
	window.setView(cam);
}

void Camera::Update(sf::RenderWindow &window) {
	// Camera no longer moves independently; it follows the player.
	window.setView(cam);
}

void Camera::FollowPlayer(sf::RenderWindow &window, const sf::Vector2f &playerWorldPos) {
	sf::Vector2f cameraCenter = cam.getCenter();
	sf::Vector2f offset = playerWorldPos - cameraCenter;

	if (std::abs(offset.x) > deadzoneWidth) {
		float moveX = offset.x > 0 ? offset.x - deadzoneWidth : offset.x + deadzoneWidth;
		cameraCenter.x += moveX;
	}

	if (std::abs(offset.y) > deadzoneHeight) {
		float moveY = offset.y > 0 ? offset.y - deadzoneHeight : offset.y + deadzoneHeight;
		cameraCenter.y += moveY;
	}

	cam.setCenter(cameraCenter);
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

void Camera::ResizeView(sf::RenderWindow &window, const int gameWidth, const int gameHeight) {
	cam.setSize(window.getSize().x, window.getSize().y);
	cam.zoom(zoomLevel); // reapply zoom after resize
	window.setView(cam);
}
