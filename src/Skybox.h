#ifndef SKYBOX_H
#define SKYBOX_H

class Mesh;

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// STL
#include <vector>

// VArch
#include "Object.h"

class Skybox : public Object
{
private:

	GLuint m_texture;
	Mesh* m_mesh;
	std::vector<const GLchar*> m_faces;

public:
	Skybox();
	~Skybox();

	void Skybox::loadCubemap();
};

#endif // SKYBOX_H