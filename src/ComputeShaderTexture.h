#ifndef COMPUTESHADERTEXTURE_H
#define COMPUTESHADERTEXTURE_H

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Texture2D.h"

class ComputeShaderTexture : public Texture2D<glm::vec4>
{
public:
	ComputeShaderTexture(int width, int height);
	~ComputeShaderTexture();

	GLuint genGLTexture(GLuint textureSlot = 0);

private:

};

#endif