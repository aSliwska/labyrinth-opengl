#pragma once

#include "pch.h"
#include "Map.h"
#include "Spherical.h"
#include "InputHandler.h"
#include "Model.h"
#include "ModelRepository.h"

class Game {
protected:
	std::shared_ptr<LockedSpherical> camera;
	std::shared_ptr<UnlockedSpherical> playerRotation;
	std::shared_ptr<sf::Vector3f> playerPosition;
	float tileSize = 1.6;
	std::unique_ptr<ModelRepository> modelRepo;
	std::shared_ptr<Map> map;
	std::unique_ptr<InputHandler> inputHandler;

	void initOpenGL();
	void reshapeScreen(const sf::Vector2u& size);
	void drawModel(const std::unique_ptr<Model>& model, float x, float y, float z);
	void drawScene();
	bool checkWinCondition();

public:
	Game();

	void run();
};