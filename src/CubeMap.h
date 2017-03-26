#ifndef CUBEMAP_H
#define CUBEMAP_H

#include "enum.h"

class HeightMap;
class NoiseProperties;

class CubeMap
{
	HeightMap* m_sides[6];
	int m_width;

	float m_min;
	float m_max;

public:

	CubeMap(int m_width);
	~CubeMap();

	HeightMap * getSide(Side side);

	void generateSimplex(NoiseProperties &noiseProperties);
	void generateSimplex(float frequency, float scale, int nbOctaves);
	HeightMap* GetUniqueHeightMap();

	void transformInterval(float newMin, float newMax);
};

#endif