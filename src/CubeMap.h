#pragma once

#include "HeightMap.h"
#include "enum.h"

class CubeMap
{
	HeightMap* m_sides[6];
	int m_width;

public:

	CubeMap(int m_width);
	~CubeMap();

	HeightMap * getSide(Side side);

	void generateSimplex(NoiseProperties noiseProperties);
	void generateSimplex(float frequency, float scale, int nbOctaves);
	HeightMap* GetUniqueHeightMap();
};

CubeMap::CubeMap(int m_width) : m_width(m_width)
{
	m_sides[0] = new HeightMap(m_width, m_width);
	m_sides[1] = new HeightMap(m_width, m_width);
	m_sides[2] = new HeightMap(m_width, m_width);
	m_sides[3] = new HeightMap(m_width, m_width);
	m_sides[4] = new HeightMap(m_width, m_width);
	m_sides[5] = new HeightMap(m_width, m_width);
}


CubeMap::~CubeMap()
{

}

HeightMap* CubeMap::getSide(Side side) 
{
	return m_sides[side];
}

void CubeMap::generateSimplex(float frequency, float scale, int nbOctaves)
{
	float halfWidth = m_width / 2.f;

	for (int i = 0; i < m_width; i++) {
		for (int j = 0; j < m_width; j++) {

			// UP
			glm::vec3 pos((i - halfWidth), halfWidth, (j - halfWidth));
			pos = /*(float)nbChunks * */glm::normalize(pos);

			float noise = Noise::noise3D(pos.x, pos.y, pos.z, frequency, scale, nbOctaves);

			m_sides[Side::UP]->set(i, j, noise);

			// DOWN
			pos = glm::normalize(glm::vec3((i - halfWidth), -halfWidth, j - halfWidth));

			noise = Noise::noise3D(pos.x, pos.y, pos.z, frequency, scale, nbOctaves);

			m_sides[Side::DOWN]->set(i, m_width - 1 - j, noise);

			// LEFT
			pos = glm::normalize(glm::vec3(-halfWidth, j - halfWidth, i - halfWidth));

			noise = Noise::noise3D(pos.x, pos.y, pos.z, frequency, scale, nbOctaves);

			m_sides[Side::LEFT]->set(i, m_width - 1 - j, noise);

			// RIGHT
			pos = glm::normalize(glm::vec3(halfWidth, j - halfWidth, i - halfWidth));

			noise = Noise::noise3D(pos.x, pos.y, pos.z, frequency, scale, nbOctaves);

			m_sides[Side::RIGHT]->set(m_width - 1 - i, m_width - 1 - j, noise);

			// FRONT
			pos = glm::normalize(glm::vec3((i - halfWidth), j - halfWidth, halfWidth));

			noise = Noise::noise3D(pos.x, pos.y, pos.z, frequency, scale, nbOctaves);

			m_sides[Side::FRONT]->set(i, m_width - 1 - j, noise);

			// BACK
			pos = glm::normalize(glm::vec3((i - halfWidth), j - halfWidth, -halfWidth));

			noise = Noise::noise3D(pos.x, pos.y, pos.z, frequency, scale, nbOctaves);

			m_sides[Side::BACK]->set(m_width - 1 - i, m_width - 1 - j, noise);
		}
	}
}

HeightMap* CubeMap::GetUniqueHeightMap()
{
	HeightMap* hm = new HeightMap(4 * m_width, 3 * m_width);

	for (int x = 0; x < m_width; x++) {
		for (int y = 0; y < m_width; y++) {

			hm->set(x + m_width, y, m_sides[0]->get(x, y)); // Up
			hm->set(x + m_width, y + m_width * 2, m_sides[1]->get(x, y)); // Down
			hm->set(x, y + m_width, m_sides[2]->get(x, y)); // Left
			hm->set(x + m_width * 2, y + m_width, m_sides[3]->get(x, y)); // Right
			hm->set(x + m_width, y + m_width, m_sides[4]->get(x, y)); // Front
			hm->set(x + m_width * 3, y + m_width, m_sides[5]->get(x, y)); // Back
		}
	}

	return hm;
}

void CubeMap::generateSimplex(NoiseProperties noiseProperties)
{
	generateSimplex(noiseProperties.frequency, noiseProperties.scale, noiseProperties.nbOctaves);
}