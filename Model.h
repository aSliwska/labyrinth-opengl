#pragma once

#include "pch.h"

struct Model {
protected:

public:
	Model(std::string dirPath, std::string fileNameWithoutExtension);
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::unordered_map<float, sf::Vector3f> colorMap;
	float xMin, yMin, zMin, xMax, yMax, zMax, xMiddle, yMiddle, zMiddle, xOffset, yOffset, zOffset;

	void setOffset(float x, float y, float z);
};