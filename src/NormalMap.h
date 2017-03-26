#ifndef NORMALMAP_H
#define NORMALMAP_H

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

	NormalMap* getPart(glm::vec2 min, glm::vec2 max);
	GLuint genGLTexture(GLuint = 0);

private:

};

#endif // !NORMALMAP_H