#pragma once

#include "pch.h"
#include "Spherical.h"
#include "Map.h"

class InputHandler {
protected:
    bool aPressed, wPressed, sPressed, dPressed;
    float modelForwardRotation, radiansToDegreesConstant;

    LockedSpherical* camera;
    UnlockedSpherical *rotation;
    sf::Vector3f* position;
    Map* map;
    float walkSpeed, tileSize, characterRadius;

public:
    InputHandler(LockedSpherical* camera, sf::Vector3f* position, UnlockedSpherical* rotation, float walkSpeed, float tileSize, Map* map, float characterRadius);

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