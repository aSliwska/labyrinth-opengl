#pragma once

#include "pch.h"
#include "Model.h"


class ModelRepository {
public:
    std::unique_ptr<Model> bush, character, ground, flag;

    ModelRepository() {}
    ModelRepository(const std::string& modelDirectory, float tileSize);
};