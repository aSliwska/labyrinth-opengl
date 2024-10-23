#pragma once

#include "pch.h"
#include "Spherical.h"
#include "Map.h"

struct InputHandler {
protected:
    bool aPressed, wPressed, sPressed, dPressed;
    float modelForwardRotation, radiansToDegreesConstant;

    Spherical* camera, * rotation;
    sf::Vector3f* position;
    Map* map;
    float walkSpeed, cameraSpeed, tileSize, characterRadius;

public:
    InputHandler(Spherical* camera, sf::Vector3f* position, Spherical* rotation, float walkSpeed, float cameraSpeed, float tileSize, Map* map, float characterRadius);

    void handleUserInput(sf::Time timeElapsed);

    bool isAPressed();
    bool isWPressed();
    bool isSPressed();
    bool isDPressed();

    void setAPressed(bool state);
    void setWPressed(bool state);
    void setSPressed(bool state);
    void setDPressed(bool state);
};