#pragma once

#include "pch.h"

class Model {
protected:
	std::shared_ptr<std::vector<float>> vertices;

public:
	Model(const std::string& dirPath, const std::string& fileNameWithoutExtension);
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::unordered_map<float, sf::Vector3f> colorMap;

	void setOffset(float x, float y, float z);
	void setVBOOffset(GLuint VBOOffset);
	std::shared_ptr<std::vector<float>> getVertices();
	unsigned long long getVerticesSize();

	float xMin, yMin, zMin, xMax, yMax, zMax, xMiddle, yMiddle, zMiddle, xOffset, yOffset, zOffset;
	GLuint VBOOffset;
};