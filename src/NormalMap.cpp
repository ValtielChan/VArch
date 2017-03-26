#include "NormalMap.h"

NormalMap::NormalMap(int width, int height) : Texture2D(width, height)
{

}

NormalMap::~NormalMap()
{
}

NormalMap *NormalMap::getPart(glm::vec2 min, glm::vec2 max)
{
	NormalMap* normalMap = new NormalMap(max.x - min.x, max.y - min.y);

	int i = 0;
	int j = 0;

	for (int x = min.x; x < max.x; x++, i++) {
		for (int y = min.y; y < max.y; y++, j++) {

			normalMap->set(i, j, get(x, y));
		}

		j = 0;
	}

	return normalMap;
}


GLuint NormalMap::genGLTexture(GLuint)
{
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	// Set our texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Load texture data form this
	float* data = new float[m_width*m_height*3]; // TODO

	int j = 0;
	for (int i = 0; i < m_width * m_height; i++) {

		data[j] = m_matrix[i].x;
		data[j+1] = m_matrix[i].y;
		data[j+2] = m_matrix[i].z;

		j += 3;
	}

	// Set gl Texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, m_width, m_height, 0, GL_RGB, GL_FLOAT, data);
	//glGenerateMipmap(GL_TEXTURE_2D);

	// Unbind
	glBindTexture(GL_TEXTURE_2D, 0);

	return m_texture;
}