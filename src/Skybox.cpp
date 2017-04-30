#include "Skybox.h"
#include <SOIL.h>


Skybox::Skybox()
{
	m_faces.push_back("ressources\\right.jpg");
	m_faces.push_back("ressources\\left.jpg");
	m_faces.push_back("ressources\\top.jpg");
	m_faces.push_back("ressources\\bottom.jpg");
	m_faces.push_back("ressources\\back.jpg");
	m_faces.push_back("ressources\\front.jpg");

	loadCubemap();
}


Skybox::~Skybox()
{
}

void Skybox::loadCubemap()
{
	glGenTextures(1, &m_texture);
	glActiveTexture(GL_TEXTURE0);

	int width, height;
	unsigned char* image;

	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
	for (GLuint i = 0; i < m_faces.size(); i++)
	{
		image = SOIL_load_image(m_faces[i], &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
			GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image
		);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}