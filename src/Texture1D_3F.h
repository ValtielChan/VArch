#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include "Texture1D.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Texture1D_3F : public Texture1D<glm::vec3>
{
public:
	Texture1D_3F(int width);
	~Texture1D_3F();

	GLuint genGLTexture();

private:

};

Texture1D_3F::Texture1D_3F(int width) : Texture1D(width)
{

}

Texture1D_3F::~Texture1D_3F()
{

}



inline GLuint Texture1D_3F::genGLTexture()
{
	float* datas = new float[m_width * 3]();

	for (int i = 0, j = 0; i < m_width; i++, j += 3) {
		datas[j] = get(i).r;
		datas[j + 1] = get(i).g;
		datas[j + 2] = get(i).b;
	}

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_1D, m_texture);

	// Set our texture parameters
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Set texture filtering
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Set gl Texture
	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB32F, m_width, 0, GL_RGB, GL_FLOAT, datas);
	//glGenerateMipmap(GL_TEXTURE_1D);

	// Unbind
	glBindTexture(GL_TEXTURE_1D, 0);

	return m_texture;
}
