#pragma once

#include "pch.h"
#include "Map.h"
#include "Spherical.h"
#include "InputHandler.h"
#include "Model.h"
#include "ModelRepository.h"

class Game {
protected:
	LockedSpherical* camera;
	UnlockedSpherical* playerRotation;
	sf::Vector3f playerPosition;
	float tileSize = 1.6;
	ModelRepository* modelRepo;
	Map* map;
	InputHandler* inputHandler;

	void initOpenGL();
	void reshapeScreen(const sf::Vector2u& size);
	void drawModel(Model* model, float x, float y, float z);
	void drawScene();
	bool checkWinCondition();

public:
	Game();

	void run();

	~Game() {
		delete camera;
		delete playerRotation;
		delete modelRepo;
		delete map;
		delete inputHandler;
	}
};