#pragma once

#include "pch.h"

struct Map
{
public:
    enum Tile { air, ground, bush, flag };

protected:
    std::vector<std::vector<Tile>> tiles;
    sf::Vector2u startPosition;
    sf::Vector2u endPosition;

    static std::vector<std::string> split(const std::string& s, char delim);

public:
    Map(sf::Vector2u startPosition, sf::Vector2u endPosition, std::vector<std::vector<Tile>> tiles);

    static Map* loadMap(std::string filePath);

    static bool isWalkable(Tile tile);
    std::vector<std::vector<Tile>> getTiles();
    unsigned int getStartX();
    unsigned int getStartZ();
    unsigned int getEndX();
    unsigned int getEndZ();
};