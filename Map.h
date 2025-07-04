#pragma once

#include "pch.h"

class Map {
public:
    enum Tile { air, ground, bush, flag };

protected:
    std::vector<std::vector<Tile>> tiles;
    sf::Vector2u startPosition;

    static std::vector<std::string> split(const std::string& s, char delim);

public:
    Map(sf::Vector2u startPosition, std::vector<std::vector<Tile>> tiles);

    static std::shared_ptr<Map> loadMap(const std::string& filePath);

    static bool isWalkable(Tile tile);
    std::vector<std::vector<Tile>> getTiles();
    unsigned int getStartX();
    unsigned int getStartZ();
};