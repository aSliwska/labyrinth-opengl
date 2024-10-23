#pragma once

#include "pch.h"
#include "Spherical.h"
#include "Map.h"

class InputHandler {
protected:
    bool aPressed, wPressed, sPressed, dPressed;
    float modelForwardRotation, radiansToDegreesConstant;

    std::shared_ptr<LockedSpherical> camera;
    std::shared_ptr<UnlockedSpherical> playerRotation;
    std::shared_ptr<sf::Vector3f> playerPosition;
    std::shared_ptr<Map> map;
    float walkSpeed, tileSize, characterRadius;

public:
    InputHandler(const std::shared_ptr<LockedSpherical>& camera, const std::shared_ptr<sf::Vector3f>& playerPosition, const std::shared_ptr<UnlockedSpherical>& playerRotation, float walkSpeed, float tileSize, const std::shared_ptr<Map>& map, float characterRadius);

    void handleUserInput(const sf::Time& timeElapsed);

    bool isAPressed();
    bool isWPressed();
    bool isSPressed();
    bool isDPressed();

    void setAPressed(bool state);
    void setWPressed(bool state);
    void setSPressed(bool state);
    void setDPressed(bool state);
};