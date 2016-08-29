#pragma once

#include "HeightMap.h"

enum CubeMapSide { UP, DOWN, LEFT, RIGHT, FRONT, BACK };

class CubeMap
{
	HeightMap* m_sides[6];
	int m_width;

public:

	CubeMap(int m_width);
	~CubeMap();

	void generateSimplex(NoiseProperties noiseProperties);
	void generateSimplex(float frequency, float scale, int nbOctaves);
};

