#include "CubeMap.h"


CubeMap::CubeMap(int m_width) : m_width(m_width)
{
	for (HeightMap* hm : m_sides)
		hm = new HeightMap(m_width, m_width);
}


CubeMap::~CubeMap()
{

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

			m_sides[CubeMapSide::UP]->set(i, j, noise); 

			// DOWN
			pos = glm::normalize(glm::vec3((i - halfWidth), -halfWidth, j - halfWidth));

			noise = Noise::noise3D(pos.x, pos.y, pos.z, frequency, scale, nbOctaves);

			m_sides[CubeMapSide::DOWN]->set(i, m_width - 1 - j, noise);

			// LEFT
			pos = glm::normalize(glm::vec3(-halfWidth, j - halfWidth, i - halfWidth));

			noise = Noise::noise3D(pos.x, pos.y, pos.z, frequency, scale, nbOctaves);

			m_sides[CubeMapSide::LEFT]->set(i, m_width - 1 - j, noise);

			// RIGHT
			pos = glm::normalize(glm::vec3(halfWidth, j - halfWidth, i - halfWidth));

			noise = Noise::noise3D(pos.x, pos.y, pos.z, frequency, scale, nbOctaves);

			m_sides[CubeMapSide::RIGHT]->set(m_width - 1 - i, m_width - 1 - j, noise);

			// FRONT
			pos = glm::normalize(glm::vec3((i - halfWidth), j - halfWidth, halfWidth));

			noise = Noise::noise3D(pos.x, pos.y, pos.z, frequency, scale, nbOctaves);

			m_sides[CubeMapSide::FRONT]->set(i, m_width - 1 - j, noise);

			// BACK
			pos = glm::normalize(glm::vec3((i - halfWidth), j - halfWidth, -halfWidth));

			noise = Noise::noise3D(pos.x, pos.y, pos.z, frequency, scale, nbOctaves);

			m_sides[CubeMapSide::BACK]->set(m_width - 1 - i, m_width - 1 - j, noise);
		}
	}
}

void CubeMap::generateSimplex(NoiseProperties noiseProperties)
{
	generateSimplex(noiseProperties.frequency, noiseProperties.scale, noiseProperties.nbOctaves);
}