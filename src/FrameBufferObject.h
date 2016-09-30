#pragma once

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

#include <iostream>

class FrameBufferObject
{
private:

	GLuint m_framebuffer;
	GLuint m_colorBuffer;
	GLuint m_renderBuffer;

public:

	FrameBufferObject(int width, int height);
	~FrameBufferObject();

	void bind();
	void unbind();
	void deleteFrameBuffer();

	GLuint frameBuffer() { return m_framebuffer; }
	GLuint colorBuffer() { return m_colorBuffer; }
	GLuint renderBuffer() { return m_renderBuffer; }
};

FrameBufferObject::FrameBufferObject(int width, int height)
{
	glGenFramebuffers(1, &m_framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

	// Generate texture
	glGenTextures(1, &m_colorBuffer);
	glBindTexture(GL_TEXTURE_2D, m_colorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Attach it to currently bound framebuffer object
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorBuffer, 0);

	glGenRenderbuffers(1, &m_renderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, m_renderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_renderBuffer);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FrameBufferObject::~FrameBufferObject()
{

}

void FrameBufferObject::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
}

void FrameBufferObject::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBufferObject::deleteFrameBuffer()
{
	glDeleteFramebuffers(1, &m_framebuffer);
}
