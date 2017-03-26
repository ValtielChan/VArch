#include "TextureRGB.h"

TextureRGB::TextureRGB(int width, int height) : Texture2D(width, height)
{

}

TextureRGB::~TextureRGB()
{

}

TextureRGB * TextureRGB::getPart(glm::vec2 min, glm::vec2 max)
{
	// TODO : Exception min max coherence

	TextureRGB* texture = new TextureRGB(max.x - min.x, max.y - min.y);

	int i = 0;
	int j = 0;

	for (int x = min.x; x < max.x; x++, i++) {
		for (int y = min.y; y < max.y; y++, j++) {

			texture->set(i, j, get(x, y));
		}

		j = 0;
	}

	return texture;
}

GLuint TextureRGB::genGLTexture(GLuint)
{
	float* datas = new float[m_width*m_height*3]();

	for (int i = 0, j = 0; i < m_width*m_height; i++, j+=3) {
		datas[j] = get(i).r;
		datas[j+1] = get(i).g;
		datas[j+2] = get(i).b;
	}

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	// Set our texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Set gl Texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, m_width, m_height, 0, GL_RGB, GL_FLOAT, datas);
	//glGenerateMipmap(GL_TEXTURE_2D);

	glBindImageTexture(0, m_texture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGB32F);

	// Unbind
	glBindTexture(GL_TEXTURE_2D, 0);

	return m_texture;
}
