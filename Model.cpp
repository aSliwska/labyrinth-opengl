
#define TINYOBJLOADER_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

#include "pch.h"
#include "Model.h"

Model::Model(std::string dirPath, std::string fileNameWithoutExtension)
{
	// read vertices
	std::string inputFileWithoutExtension = dirPath + "/" + fileNameWithoutExtension;

	tinyobj::ObjReader reader;
	tinyobj::ObjReaderConfig reader_config;
	reader_config.mtl_search_path = dirPath;

	if (!reader.ParseFromFile(inputFileWithoutExtension + ".obj", reader_config)) {
		if (!reader.Error().empty()) {
			std::cerr << "TinyObjReader: " << reader.Error();
		}
		exit(1);
	}

	if (!reader.Warning().empty()) {
		std::cout << "TinyObjReader: " << reader.Warning();
	}
	
	attrib = reader.GetAttrib();
	shapes = reader.GetShapes();

	// read colors
	int x, y, n;
	std::string imagePath = inputFileWithoutExtension + ".png";

	float* data = stbi_loadf(imagePath.c_str(), &x, &y, &n, 0);

	if (data == NULL)
	{
		std::cout << "Failed to read texture image info." << std::endl;
		exit(1);
	}

	// only reads palettes 1 px in height
	for (int i = 0; i < attrib.texcoords.size(); i += 2)
	{
		float textureXPosition = attrib.texcoords[i];
		int textureXIndex = std::floor(textureXPosition * x) * n;

		colorMap[textureXPosition] = sf::Vector3f(data[textureXIndex], data[textureXIndex + 1], data[textureXIndex + 2]);
	}

	stbi_image_free(data);

	// calculate coordinates for texture alignment
	xMin = zMin = std::numeric_limits<float>::max();
	xMax = zMax = std::numeric_limits<float>::min();

	// Loop over shapes
	for (size_t s = 0; s < shapes.size(); s++)
	{
		// Loop over faces (polygon)
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
		{
			size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

			// Loop over vertices in the face.
			for (size_t v = 0; v < fv; v++)
			{
				// access to vertex
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
				tinyobj::real_t vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
				tinyobj::real_t vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
				tinyobj::real_t vz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];

				if (vx < xMin)
					xMin = vx;
				else if (vx > xMax)
					xMax = vx;

				if (vy < yMin)
					yMin = vy;
				else if (vy > yMax)
					yMax = vy;

				if (vz < zMin)
					zMin = vz;
				else if (vz > zMax)
					zMax = vz;
			}
			index_offset += fv;
		}
	}

	xMiddle = (xMax - xMin) * 0.5 + xMin;
	yMiddle = (yMax - yMin) * 0.5 + yMin;
	zMiddle = (zMax - zMin) * 0.5 + zMin;
}

void Model::setOffset(float x, float y, float z)
{
	xOffset = x;
	yOffset = y;
	zOffset = z;
}