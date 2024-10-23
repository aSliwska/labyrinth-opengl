#pragma once

#include "pch.h"
#include "Model.h"


class ModelRepository {
public:
    Model* bush, * character, * ground, * flag;

    ModelRepository();
    ModelRepository(const std::string& modelDirectory, float tileSize);

    ~ModelRepository();
};