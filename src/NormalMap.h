#pragma once

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Texture2D.h"

class NormalMap : public Texture2D<glm::vec3>
{
public:
	NormalMap(int width, int height);
	~NormalMap();

	GLuint genGLTexture();

private:

};

NormalMap::NormalMap(int width, int height) : Texture2D(width, height)
{

}

NormalMap::~NormalMap()
{
}

GLuint NormalMap::genGLTexture()
{
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	// Set our texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load texture data form this
	float* data = new float[m_width*m_height*3]; // TODO

	int j = 0;
	for (int i = 0; i < m_width * m_height; i++) {

		data[j] = m_matrix[i].x;
		data[j+1] = m_matrix[i].y;
		data[j+2] = m_matrix[i].z;

		j += 3;
	}

	// Set gl Texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, m_width, m_height, 0, GL_RGB, GL_FLOAT, data);
	//glGenerateMipmap(GL_TEXTURE_2D);

	// Unbind
	glBindTexture(GL_TEXTURE_2D, 0);

	return m_texture;
}