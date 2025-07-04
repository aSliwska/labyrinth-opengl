#include "pch.h"
#include "InputHandler.h"

InputHandler::InputHandler(const std::shared_ptr<LockedSpherical>& camera, const std::shared_ptr<sf::Vector3f>& playerPosition, const std::shared_ptr<UnlockedSpherical>& playerRotation, float walkSpeed, float tileSize, const std::shared_ptr<Map>& map, float characterRadius)
{
    aPressed = wPressed = sPressed = dPressed = false;
    modelForwardRotation = 90;
    radiansToDegreesConstant = 57.29577;

    this->camera = camera;
    this->playerPosition = playerPosition;
    this->playerRotation = playerRotation;
    this->walkSpeed = walkSpeed;
    this->tileSize = tileSize;
    this->map = map;
    this->characterRadius = characterRadius;
}

void InputHandler::handleUserInput(const sf::Time& timeElapsed)
{
    float time = timeElapsed.asMilliseconds();

    short right = 0;
    short forward = 0;

    if (aPressed) right++;
    if (dPressed) right--;
    if (wPressed) forward++;
    if (sPressed) forward--;

    if (forward != 0 || right != 0) 
    {
        float curWalkSpeed = walkSpeed;

        if (forward != 0 && right != 0) 
            curWalkSpeed /= 1.4142;

        float distanceWalkedX = 0;
        float distanceWalkedZ = 0;

        if (forward != 0) 
        {
            distanceWalkedX -= (camera->getX() / camera->getDistance()) * forward * curWalkSpeed * time;
            distanceWalkedZ -= (camera->getZ() / camera->getDistance()) * forward * curWalkSpeed * time;
        }

        if (right != 0)
        {
            distanceWalkedX -= (camera->getZ() / camera->getDistance()) * right * curWalkSpeed * time;
            distanceWalkedZ -= (-camera->getX() / camera->getDistance()) * right * curWalkSpeed * time;
        }

        int oldMapCoordX = std::floor(playerPosition->x / tileSize);
        int oldMapCoordZ = std::floor(playerPosition->z / tileSize);

        int walkDirectionXSign = (distanceWalkedX < 0) ? -1 : ((distanceWalkedX > 0) ? 1 : 0);
        int walkDirectionZSign = (distanceWalkedZ < 0) ? -1 : ((distanceWalkedZ > 0) ? 1 : 0);

        float newPositionToCheckX = playerPosition->x + distanceWalkedX + walkDirectionXSign * characterRadius;
        float newPositionToCheckZ = playerPosition->z + distanceWalkedZ + walkDirectionZSign * characterRadius;

        int newMapCoordToCheckX = std::floor(newPositionToCheckX / tileSize);
        int newMapCoordToCheckZ = std::floor(newPositionToCheckZ / tileSize);

        // this many checks so character doesn't clip through corners
        if (((oldMapCoordX == newMapCoordToCheckX) && (oldMapCoordZ == newMapCoordToCheckZ)) ||
            (Map::isWalkable(map->getTiles()[newMapCoordToCheckZ][newMapCoordToCheckX]) &&
             Map::isWalkable(map->getTiles()[newMapCoordToCheckZ][oldMapCoordX]) &&
             Map::isWalkable(map->getTiles()[oldMapCoordZ][newMapCoordToCheckX]))
        ) 
        {
            playerPosition->x += distanceWalkedX;
            playerPosition->z += distanceWalkedZ;
        }
        else 
        {
            // bounce back
            int mapCoordToCheckX = newMapCoordToCheckX;
            float rayStep = walkDirectionXSign * 0.001;

            while (distanceWalkedX * walkDirectionXSign > 0) {
                if (Map::isWalkable(map->getTiles()[newMapCoordToCheckZ][mapCoordToCheckX]) && Map::isWalkable(map->getTiles()[oldMapCoordZ][mapCoordToCheckX])) {
                    playerPosition->x += distanceWalkedX;
                    break;
                }
                distanceWalkedX -= rayStep;
                newPositionToCheckX -= rayStep;
                mapCoordToCheckX = std::floor(newPositionToCheckX / tileSize);
            }

            int mapCoordToCheckZ = newMapCoordToCheckZ;
            rayStep = walkDirectionZSign * 0.001;

            while (distanceWalkedZ * walkDirectionZSign > 0) {
                if (Map::isWalkable(map->getTiles()[mapCoordToCheckZ][newMapCoordToCheckX]) && Map::isWalkable(map->getTiles()[mapCoordToCheckZ][oldMapCoordX])) {
                    playerPosition->z += distanceWalkedZ;
                    break;
                }
                distanceWalkedZ -= rayStep;
                newPositionToCheckZ -= rayStep;
                mapCoordToCheckZ = std::floor(newPositionToCheckZ / tileSize);
            }
        }

        playerRotation->setTheta(-radiansToDegreesConstant * camera->getTheta() - modelForwardRotation + ((forward == -1) ? 180 : 0) + right * ((forward != 0) ? 45 * forward : 90));
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
