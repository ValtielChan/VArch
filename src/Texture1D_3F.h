#ifndef TEXTURE1D_3F_H
#define TEXTURE1D_3F_H

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Texture1D.h"

class Texture1D_3F : public Texture1D<glm::vec3>
{
public:
	Texture1D_3F(int width);
	~Texture1D_3F();

	GLuint genGLTexture(GLuint = 0);

private:

};

#endif // !TEXTURE1D_3F_H
