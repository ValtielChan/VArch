#ifndef FRAMEBUFFEROBJECT_H
#define FRAMEBUFFEROBJECT_H

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

#endif // !FRAMEBUFFEROBJECT_H