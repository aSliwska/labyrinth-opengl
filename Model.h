#pragma once

#include "pch.h"

class Model {
protected:

public:
	Model(const std::string& dirPath, const std::string& fileNameWithoutExtension);
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::unordered_map<float, sf::Vector3f> colorMap;
	float xMin, yMin, zMin, xMax, yMax, zMax, xMiddle, yMiddle, zMiddle, xOffset, yOffset, zOffset;

	void setOffset(float x, float y, float z);
};