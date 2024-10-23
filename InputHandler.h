#pragma once

#include "pch.h"
#include "Spherical.h"
#include "Map.h"

struct InputHandler {
protected:
    bool aPressed, wPressed, sPressed, dPressed, leftPressed, rightPressed;
    float modelForwardRotation, radiansToDegreesConstant;

public:
    InputHandler();

    void handleUserInput(Spherical* camera, sf::Vector3f* position, Spherical* rotation, sf::Time timeElapsed, float walkSpeed, float cameraSpeed, float tileSize, Map* map, float characterRadius);

    bool isAPressed();
    bool isWPressed();
    bool isSPressed();
    bool isDPressed();
    bool isLeftPressed();
    bool isRightPressed();

    void setAPressed(bool state);
    void setWPressed(bool state);
    void setSPressed(bool state);
    void setDPressed(bool state);
    void setLeftPressed(bool state);
    void setRightPressed(bool state);
};