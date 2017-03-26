#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

// STL
#include <algorithm>

#include "Texture2D.h"

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>

class NormalMap;
class TextureRGB;
class ColorTable;
struct NoiseProperties;

#define EXCEPTION_ID "[HeightMap]"

class HeightMap : public Texture2D<float>
{

public:

	HeightMap(int width, int height);
	~HeightMap();

	void set(int x, int y, float value);
	float get(int x, int y);

	void generateSimplex(NoiseProperties *np);
	NormalMap* generateNormalMap();
	TextureRGB* generateColorMap(const ColorTable &colorTable);

	void transformInterval(float newMin, float newMax);
	void transformInterval(float newMin, float newMax, float curMin, float curMax);
	HeightMap* getPart(glm::vec2 min, glm::vec2 max);

	GLuint genGLTexture(GLuint = 0);
};

#endif // !HEIGHTMAP_H