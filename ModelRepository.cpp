#include "ModelRepository.h"

ModelRepository::ModelRepository()
{
    bush = character = ground = flag = NULL;
}

ModelRepository::ModelRepository(const std::string& modelDirectory, float tileSize) {
    bush = new Model(modelDirectory, "bush");
    bush->setOffset(-bush->xMin, -bush->yMin, -bush->zMin);

    character = new Model(modelDirectory, "character_alt");
    character->setOffset(-character->xMin - (character->xMax - character->xMiddle), -character->yMin, -character->zMin - (character->zMax - character->zMiddle));

    ground = new Model(modelDirectory, "flat_ground");
    ground->setOffset(-ground->xMin, -ground->yMin - ground->yMax, -ground->zMin);

    flag = new Model(modelDirectory, "flag");
    flag->setOffset(-flag->xMin + tileSize / 2 - (flag->xMiddle - flag->xMin), -flag->yMin, -flag->zMin + tileSize / 2 - (flag->zMiddle - flag->zMin));
}

ModelRepository::~ModelRepository() {
    delete bush;
    delete character;
    delete ground;
    delete flag;
}