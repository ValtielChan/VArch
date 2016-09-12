#pragma once

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// STL
#include <algorithm>

// Engine
#include "NormalMap.h"
#include "Noise.h"

class HeightMap : public Texture2D<float>
{
private:

	float m_min;
	float m_max;

public:

	HeightMap(int width, int height);
	~HeightMap();

	void set(int x, int y, float value);
	float get(int x, int y);

	float min() { return m_min; }
	float max() { return m_max; }

	void generateSimplex(NoiseProperties *np);
	NormalMap* generateNormalMap();
	void transformInterval(float newMin, float newMax);

	GLuint genGLTexture();
};

HeightMap::HeightMap(int width, int height) : Texture2D(width, height), m_min(2542.f), m_max(-2542.f)
{}


HeightMap::~HeightMap()
{}

void HeightMap::set(int x, int y, float value)
{
	m_matrix[y * m_width + x] = value;

	m_min = std::fmin(value, m_min);
	m_max = std::fmax(value, m_max);
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