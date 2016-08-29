#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shaders.h"

class Renderer
{
	GLuint m_quadVAO;

public:
	Renderer();
	~Renderer();

	virtual void render() = 0;

	void renderToQuad(GLuint texture);
};

