#include "pch.h"
#include "InputHandler.h"

InputHandler::InputHandler() 
{
    aPressed = wPressed = sPressed = dPressed = leftPressed = rightPressed = false;
    modelForwardRotation = 90;
    radiansToDegreesConstant = 57.29577;
}

void InputHandler::handleUserInput(Spherical* camera, sf::Vector3f* position, Spherical* rotation, sf::Time timeElapsed, float walkSpeed, float cameraSpeed, float tileSize, Map* map, float characterRadius)
{
    float time = timeElapsed.asMilliseconds();

    short right = 0;
    short forward = 0;
    short cameraRight = 0;

    if (aPressed) right++;
    if (dPressed) right--;
    if (wPressed) forward++;
    if (sPressed) forward--;
    if (rightPressed) cameraRight--;
    if (leftPressed) cameraRight++;

    camera->theta -= cameraRight * cameraSpeed / time;

    if (forward != 0 && right != 0) 
    {
        walkSpeed /= 1.4142;
    }

    float distanceWalkedX = 0;
    float distanceWalkedZ = 0;

    if (forward != 0) 
    {
        distanceWalkedX -= (camera->getX() / camera->distance) * forward * walkSpeed / time;
        distanceWalkedZ -= (camera->getZ() / camera->distance) * forward * walkSpeed / time;
    }

    if (right != 0)
    {
        distanceWalkedX -= (camera->getZ() / camera->distance) * right * walkSpeed / time;
        distanceWalkedZ -= (-camera->getX() / camera->distance) * right * walkSpeed / time;
    }

    int oldMapPositionX = std::floor(position->x / tileSize);
    int oldMapPositionZ = std::floor(position->z / tileSize);

    int walkDirectionXSign = (distanceWalkedX < 0) ? -1 : ((distanceWalkedX > 0) ? 1 : 0);
    int walkDirectionZSign = (distanceWalkedZ < 0) ? -1 : ((distanceWalkedZ > 0) ? 1 : 0);

    int newMapPositionToCheckX = std::floor((position->x + distanceWalkedX) / tileSize + walkDirectionXSign * characterRadius);
    int newMapPositionToCheckZ = std::floor((position->z + distanceWalkedZ) / tileSize + walkDirectionZSign * characterRadius);

    // this many checks so character doesn't clip through corners
    if (map->getTiles()[newMapPositionToCheckZ][newMapPositionToCheckX] == Map::Tile::ground &&
        map->getTiles()[newMapPositionToCheckZ][oldMapPositionX] == Map::Tile::ground &&
        map->getTiles()[oldMapPositionZ][newMapPositionToCheckX] == Map::Tile::ground) 
    {
        position->x += distanceWalkedX;
        position->z += distanceWalkedZ;
    }
    else 
    {
        // bounce back
        if (map->getTiles()[oldMapPositionZ][newMapPositionToCheckX] == Map::Tile::ground) {
            position->x += distanceWalkedX;
        }
        else {
            position->x = (oldMapPositionX + walkDirectionXSign) * tileSize - walkDirectionXSign * 0.001;
        }

        if (map->getTiles()[newMapPositionToCheckZ][oldMapPositionX] == Map::Tile::ground) {
            position->z += distanceWalkedZ;
        }
        else {
            position->z = (oldMapPositionZ+ walkDirectionZSign) * tileSize - walkDirectionZSign * 0.001;
        }
    }

    if (right != 0 || forward != 0)
    {
		rotation->theta = -radiansToDegreesConstant * camera->theta - modelForwardRotation + ((forward == -1) ? 180 : 0) + right * ((forward != 0) ? 45*forward : 90);
    }
}

bool InputHandler::isAPressed() 
{
    return aPressed;
}

bool InputHandler::isWPressed() 
{
    return wPressed;
}

bool InputHandler::isSPressed() 
{
    return sPressed;
}

bool InputHandler::isDPressed() 
{
    return dPressed;
}

bool InputHandler::isLeftPressed() 
{
    return leftPressed;
}

bool InputHandler::isRightPressed() 
{
    return rightPressed;
}

void InputHandler::setAPressed(bool state) 
{
    aPressed = state;
}

void InputHandler::setWPressed(bool state) 
{
    wPressed = state;
}

void InputHandler::setSPressed(bool state) 
{
    sPressed = state;
}

void InputHandler::setDPressed(bool state) 
{
    dPressed = state;
}

void InputHandler::setLeftPressed(bool state) 
{
    leftPressed = state;
}

void InputHandler::setRightPressed(bool state) 
{
    rightPressed = state;
}