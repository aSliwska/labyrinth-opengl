#include "pch.h"
#include "Map.h"

Map::Map(sf::Vector2u startPosition, std::vector<std::vector<Map::Tile>> tiles) 
{
    this->startPosition = startPosition;
    this->tiles = tiles;
}

std::vector<std::string> Map::split(const std::string& s, char delim) {
    std::vector<std::string> result;
    std::stringstream ss(s);
    std::string item;

    while (getline(ss, item, delim)) {
        result.push_back(item);
    }

    return result;
}

std::shared_ptr<Map> Map::loadMap(const std::string& filePath) {
    sf::Vector2u startPosition;
    std::vector<std::vector<Map::Tile>> tiles;

    std::ifstream file(filePath);
    std::string line;

    // parameters
    std::getline(file, line);
    std::vector<std::string> startPositionParams = split(line, ',');
    startPosition = sf::Vector2u(std::stoul(startPositionParams.at(0)) + 1, std::stoul(startPositionParams.at(1)) + 2);

    // map
    tiles.push_back(std::vector<Map::Tile>());

    unsigned int maxLineLength = 0;
    unsigned int y = 0;
    while (std::getline(file, line))
    {
        unsigned int x = 0;
        std::vector<Map::Tile> row;

        if (maxLineLength < line.length())
            maxLineLength = line.length();

        row.push_back(Map::Tile::air);
        for (char& c : line)
        {
            if (c == 'X') 
                row.push_back(Map::Tile::bush);
            else if (c == 'O')
                row.push_back(Map::Tile::ground);
            else if (c == '1')
                row.push_back(Map::Tile::flag);
            else 
                row.push_back(Map::Tile::air);
            x++;
        }

        tiles.push_back(row);
        y++;
    }

    tiles.push_back(std::vector<Map::Tile>());

    for (auto& row : tiles) {
        for (int i = row.size(); i < maxLineLength + 2; i++) {
            row.push_back(Map::Tile::air);
        }
    }

    return std::shared_ptr<Map>(new Map(startPosition, tiles));
}

bool Map::isWalkable(Tile tile)
{
    return (tile == Map::Tile::ground) || (tile == Map::Tile::flag);
}

std::vector<std::vector<Map::Tile>> Map::getTiles() 
{
    return tiles;
}

unsigned int Map::getStartX() 
{
    return startPosition.x;
}

unsigned int Map::getStartZ() 
{
    return startPosition.y;
}
