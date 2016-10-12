#pragma once

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// STL
#include <algorithm>

// Engine
#include "NormalMap.h"
#include "TextureRGB.h"
#include "Noise.h"
#include "ColorTable.h"

#define EXCEPTION_ID "[HeightMap]"

class HeightMap : public Texture2D<float>
{
private:


public:

	HeightMap(int width, int height);
	~HeightMap();

	void set(int x, int y, float value);
	float get(int x, int y);

	void generateSimplex(NoiseProperties *np);
	NormalMap* generateNormalMap();
	TextureRGB* generateColorMap(const ColorTable &colorTable);

	void transformInterval(float newMin, float newMax);
	HeightMap* getPart(glm::vec2 min, glm::vec2 max);

	GLuint genGLTexture();
};

HeightMap::HeightMap(int width, int height) : Texture2D(width, height)
{}


HeightMap::~HeightMap()
{}

void HeightMap::set(int x, int y, float value)
{
	try {
		if (x >= 0 || x < m_width || y >= 0 || y < m_height) {

			m_matrix[y * m_width + x] = value;

			m_min = std::fmin(value, m_min);
			m_max = std::fmax(value, m_max);
		}
		else {
			throw Exception(ErrorLevel::ERROR, std::string(EXCEPTION_ID) + " Out of bound texel access");
		}
	}
	catch (Exception const &e) {
		e.print();
	}
}

float HeightMap::get(int x, int y)
{
	if (x < 0 || x >= m_width || y < 0 || y > m_height)
		return 0;
	else
		return m_matrix[y * m_width + x];
}

void HeightMap::generateSimplex(NoiseProperties *np)
{
	for (int x = 0; x < m_width; x++) {
		for (int y = 0; y < m_height; y++) {

			float noise = Noise::noise2D((float)x / m_width, (float)y / m_width, np);
			set(x, y, noise);

			//std::cout << noise << " ";
		}
	}

	genGLTexture();
}

NormalMap * HeightMap::generateNormalMap()
{
	NormalMap* normalMap = new NormalMap(m_width, m_height);

	for (int x = 0; x < m_width; x++) {
		for (int y = 0; y < m_height; y++) {

			float center = get(x, y);
			float up = get(x, y - 1);
			float down = get(x, y + 1);
			float left = get(x - 1, y);
			float right = get(x + 1, y);
			float upleft = get(x - 1, y - 1);
			float upright = get(x + 1, y - 1);
			float downleft = get(x - 1, y + 1);
			float downright = get(x + 1, y + 1);

			float dx = 2 * (right - left) - upleft - downleft + upright + downright;
			float dy = 1.f / 2.f;
			float dz = 2 * (up - down) - upleft - upright + downleft + downright;

			normalMap->set(x, y, glm::normalize(glm::vec3(dx, dy, dz)));
			//normalMap->set(x, y, glm::vec3(0, 0, 1));
		}
	}

	normalMap->genGLTexture();

	return normalMap;
}

TextureRGB * HeightMap::generateColorMap(const ColorTable &colorTable)
{
	TextureRGB* colorMap = new TextureRGB(m_width, m_height);

	for (int x = 0; x < m_width; x++) {
		for (int y = 0; y < m_height; y++) {

			int index = ((get(x, y) - m_min) / (m_max - m_min)) * colorTable.size();
			colorMap->set(x, y, colorTable.color(index));
		}
	}

	return colorMap;
}

void HeightMap::transformInterval(float newMin, float newMax)
{
	m_max -= m_min;

	/*std::cout << m_min << " ";
	std::cout << m_max << " ";*/

	for (int i = 0; i < m_width*m_height; i++) {
		m_matrix[i] -= m_min;
		m_matrix[i] /= m_max;

		m_matrix[i] *= (newMax - newMin);
		m_matrix[i] += newMin;

		//std::cout << m_matrix[i] << " ";
	}

	m_min = newMin;
	m_max = newMax;

	genGLTexture();
}

HeightMap * HeightMap::getPart(glm::vec2 min, glm::vec2 max)
{
	// TODO : Exception min max coherence

	HeightMap* hm = new HeightMap(max.x - min.x, max.y - min.y);

	int i = 0;
	int j = 0;

	for (int x = min.x; x < max.x; x++, i++) {
		for (int y = min.y; y < max.y; y++, j++) {

			hm->set(i, j, get(x, y));
		}

		j = 0;
	}

	// TODO : shade set min max of part
	hm->setMin(m_min);
	hm->setMax(m_max);

	return hm;
}

GLuint HeightMap::genGLTexture()
{
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	// Set our texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Set gl Texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, m_width, m_height, 0, GL_RED, GL_FLOAT, m_matrix);
	//glGenerateMipmap(GL_TEXTURE_2D);

	// Unbind
	glBindTexture(GL_TEXTURE_2D, 0);

	return m_texture;
}