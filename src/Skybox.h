#pragma once

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

#include <vector>

class Skybox
{
private:

	GLuint m_texture;
	std::vector<const GLchar*> m_faces;

public:
	Skybox();
	~Skybox();

	void Skybox::loadCubemap();
};

