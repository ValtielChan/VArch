#ifndef TEXTURERGB_H
#define TEXTURERGB_H

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Texture2D.h"

class TextureRGB : public Texture2D<glm::vec3>
{
public:
	TextureRGB(int width, int height);
	~TextureRGB();

	TextureRGB * getPart(glm::vec2 min, glm::vec2 max);
	GLuint genGLTexture(GLuint = 0);

private:

};

#endif // !TEXTURERGB_H