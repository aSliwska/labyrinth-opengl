#include "pch.h"
#include "Map.h"

Map::Map(sf::Vector2u startPosition, sf::Vector2u endPosition, std::vector<std::vector<Map::Tile>> tiles) 
{
    this->startPosition = startPosition;
    this->endPosition = endPosition;
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
    sf::Vector2u endPosition;
    std::vector<std::vector<Map::Tile>> tiles;

    std::ifstream file(filePath);
    std::string str;

    // parameters
    std::getline(file, str);
    std::vector<std::string> params = split(str, ';');
    std::vector<std::string> startPositionParams = split(params.at(0), ',');
    std::vector<std::string> endPositionParams = split(params.at(1), ',');

    startPosition = sf::Vector2u(1 + std::stoul(startPositionParams.at(0)), 1 + std::stoul(startPositionParams.at(1)));
    endPosition = sf::Vector2u(1 + std::stoul(endPositionParams.at(0)), 1 + std::stoul(endPositionParams.at(1)));

    // map
    tiles.push_back(std::vector<Map::Tile>());

    unsigned int maxLineLength = 0;
    unsigned int y = 0;
    while (std::getline(file, str))
    {
        unsigned int x = 0;
        std::vector<Map::Tile> row;

        if (maxLineLength < str.length())
            maxLineLength = str.length();

        row.push_back(Map::Tile::air);
        for (char& c : str)
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

    return std::shared_ptr<Map>(new Map(startPosition, endPosition, tiles));
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

unsigned int Map::getEndX() 
{
    return endPosition.x;
}

unsigned int Map::getEndZ() 
{
    return endPosition.y;
}