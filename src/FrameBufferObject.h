#ifndef FRAMEBUFFEROBJECT_H
#define FRAMEBUFFEROBJECT_H

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

#include <iostream>

class FrameBufferObject
{
private:

	int m_width;
	int m_height;

	GLuint m_framebuffer;
	GLuint m_texture;
	GLuint m_renderBuffer;

public:

	FrameBufferObject(int width, int height, GLuint internalFormat, GLenum format, GLenum type, GLenum attachement);
	~FrameBufferObject();

	void bind();
	void unbind();
	void createRenderBuffer();
	void deleteFrameBuffer();

	GLuint frameBuffer() { return m_framebuffer; }
	GLuint texture() { return m_texture; }
	GLuint renderBuffer() { return m_renderBuffer; }
};

#endif // !FRAMEBUFFEROBJECT_H