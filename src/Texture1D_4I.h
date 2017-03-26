#ifndef TEXTURE1D_4I_H
#define TEXTURE1D_4I_H

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Texture1D.h"

class Texture1D_4I : public Texture1D<glm::vec4>
{
public:
	Texture1D_4I(int width);
	~Texture1D_4I();

	GLuint genGLTexture(GLuint = 0);

private:

};

#endif // !TEXTURE1D_4I_H