#pragma once

#include "HeightMap.h"
#include "enum.h"

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

CubeMap::CubeMap(int m_width) : m_width(m_width), m_min(0.f), m_max(0.f)
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

/*
	Imagine the cube map as an actual normilized cube, 
	each position on the cube is projected on a sphere for noise calculation
	Then we can map a coherent noise around a sphere
*/
void CubeMap::generateSimplex(float frequency, float scale, int nbOctaves)
{
	float halfWidth = m_width / 2.f;
	float noise;
	glm::vec3 pos;

	for (int i = 0; i < m_width; i++) {
		for (int j = 0; j < m_width; j++) {

			// UP
			pos = glm::normalize(glm::vec3(i - halfWidth, halfWidth, j - halfWidth));

			noise = Noise::noise3D(pos.x, pos.y, pos.z, frequency, scale, nbOctaves);
			m_sides[Side::UP]->set(i, j, noise);
			m_min = std::min(m_min, noise);
			m_max = std::max(m_max, noise);

			// DOWN
			pos = glm::normalize(glm::vec3((i - halfWidth), -halfWidth, j - halfWidth));

			noise = Noise::noise3D(pos.x, pos.y, pos.z, frequency, scale, nbOctaves);
			m_sides[Side::DOWN]->set(i, m_width - 1 - j, noise);
			m_min = std::min(m_min, noise);
			m_max = std::max(m_max, noise);

			// LEFT
			pos = glm::normalize(glm::vec3(-halfWidth, j - halfWidth, i - halfWidth));

			noise = Noise::noise3D(pos.x, pos.y, pos.z, frequency, scale, nbOctaves);
			m_sides[Side::LEFT]->set(i, m_width - 1 - j, noise);
			m_min = std::min(m_min, noise);
			m_max = std::max(m_max, noise);

			// RIGHT
			pos = glm::normalize(glm::vec3(halfWidth, j - halfWidth, i - halfWidth));

			noise = Noise::noise3D(pos.x, pos.y, pos.z, frequency, scale, nbOctaves);
			m_sides[Side::RIGHT]->set(m_width - 1 - i, m_width - 1 - j, noise);
			m_min = std::min(m_min, noise);
			m_max = std::max(m_max, noise);

			// FRONT
			pos = glm::normalize(glm::vec3((i - halfWidth), j - halfWidth, halfWidth));

			noise = Noise::noise3D(pos.x, pos.y, pos.z, frequency, scale, nbOctaves);
			m_sides[Side::FRONT]->set(i, m_width - 1 - j, noise);
			m_min = std::min(m_min, noise);
			m_max = std::max(m_max, noise);

			// BACK
			pos = glm::normalize(glm::vec3((i - halfWidth), j - halfWidth, -halfWidth));

			noise = Noise::noise3D(pos.x, pos.y, pos.z, frequency, scale, nbOctaves);
			m_sides[Side::BACK]->set(m_width - 1 - i, m_width - 1 - j, noise);
			m_min = std::min(m_min, noise);
			m_max = std::max(m_max, noise);
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

void CubeMap::transformInterval(float newMin, float newMax)
{
	int lawl = 0;

	for (int i = 0; i < 6; i++)
		m_sides[i]->transformInterval(newMin, newMax, m_min, m_max);
}

void CubeMap::generateSimplex(NoiseProperties &noiseProperties)
{
	generateSimplex(noiseProperties.frequency, noiseProperties.scale, noiseProperties.nbOctaves);
}