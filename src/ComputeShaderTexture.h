#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include "Texture2D.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class ComputeShaderTexture : public Texture2D<glm::vec4>
{
public:
	ComputeShaderTexture(int width, int height);
	~ComputeShaderTexture();

	GLuint genGLTexture();

private:

};

ComputeShaderTexture::ComputeShaderTexture(int width, int height) : Texture2D(width, height)
{
}

ComputeShaderTexture::~ComputeShaderTexture()
{
}

GLuint ComputeShaderTexture::genGLTexture()
{
	glGenTextures(1, &m_texture);
	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_2D, m_texture);

	// Set our texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_width, m_height, 0, GL_RGBA, GL_FLOAT,
		NULL);
	glBindImageTexture(0, m_texture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

	// Unbind
	glBindTexture(GL_TEXTURE_2D, 0);

	return m_texture;
}