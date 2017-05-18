#include <SOIL.h>

#include "Skybox.h"
#include "Mesh.h"
#include "SkyboxMaterial.h"

Skybox::Skybox()
{
	std::cout << "Build Skybox" << std::endl;

	// Load images

	m_faces.push_back("ressources/right.png");
	m_faces.push_back("ressources/left.png");
	m_faces.push_back("ressources/up.png");
	m_faces.push_back("ressources/down.png");
	m_faces.push_back("ressources/back.png");
	m_faces.push_back("ressources/front.png");

	loadCubemap();

	// Material

	SkyboxMaterial *mat = new SkyboxMaterial();
	mat->cubemap = m_texture;

	// VAO & Mesh Setup

	std::vector<Vertex> vertices;
     
	vertices.push_back(Vertex(glm::vec3(-1.0f,  1.0f, -1.0f)));
	vertices.push_back(Vertex(glm::vec3(-1.0f, -1.0f, -1.0f)));
	vertices.push_back(Vertex(glm::vec3(1.0f, -1.0f, -1.0f)));
	vertices.push_back(Vertex(glm::vec3(1.0f, -1.0f, -1.0f)));
	vertices.push_back(Vertex(glm::vec3(1.0f,  1.0f, -1.0f)));
	vertices.push_back(Vertex(glm::vec3(-1.0f,  1.0f, -1.0f)));

	vertices.push_back(Vertex(glm::vec3(-1.0f, -1.0f,  1.0f)));
	vertices.push_back(Vertex(glm::vec3(-1.0f, -1.0f, -1.0f)));
	vertices.push_back(Vertex(glm::vec3(-1.0f,  1.0f, -1.0f)));
	vertices.push_back(Vertex(glm::vec3(-1.0f,  1.0f, -1.0f)));
	vertices.push_back(Vertex(glm::vec3(-1.0f,  1.0f,  1.0f)));
	vertices.push_back(Vertex(glm::vec3(-1.0f, -1.0f,  1.0f)));

	vertices.push_back(Vertex(glm::vec3(1.0f, -1.0f, -1.0f)));
	vertices.push_back(Vertex(glm::vec3(1.0f, -1.0f,  1.0f)));
	vertices.push_back(Vertex(glm::vec3(1.0f,  1.0f,  1.0f)));
	vertices.push_back(Vertex(glm::vec3(1.0f,  1.0f,  1.0f)));
	vertices.push_back(Vertex(glm::vec3(1.0f,  1.0f, -1.0f)));
	vertices.push_back(Vertex(glm::vec3(1.0f, -1.0f, -1.0f)));

	vertices.push_back(Vertex(glm::vec3(-1.0f, -1.0f,  1.0f)));
	vertices.push_back(Vertex(glm::vec3(-1.0f,  1.0f,  1.0f)));
	vertices.push_back(Vertex(glm::vec3(1.0f,  1.0f,  1.0f)));
	vertices.push_back(Vertex(glm::vec3(1.0f,  1.0f,  1.0f)));
	vertices.push_back(Vertex(glm::vec3(1.0f, -1.0f,  1.0f)));
	vertices.push_back(Vertex(glm::vec3(-1.0f, -1.0f,  1.0f)));

	vertices.push_back(Vertex(glm::vec3(-1.0f,  1.0f, -1.0f)));
	vertices.push_back(Vertex(glm::vec3(1.0f,  1.0f, -1.0f)));
	vertices.push_back(Vertex(glm::vec3(1.0f,  1.0f,  1.0f)));
	vertices.push_back(Vertex(glm::vec3(1.0f,  1.0f,  1.0f)));
	vertices.push_back(Vertex(glm::vec3(-1.0f,  1.0f,  1.0f)));
	vertices.push_back(Vertex(glm::vec3(-1.0f,  1.0f, -1.0f)));

	vertices.push_back(Vertex(glm::vec3(-1.0f, -1.0f, -1.0f)));
	vertices.push_back(Vertex(glm::vec3(-1.0f, -1.0f,  1.0f)));
	vertices.push_back(Vertex(glm::vec3(1.0f, -1.0f, -1.0f)));
	vertices.push_back(Vertex(glm::vec3(1.0f, -1.0f, -1.0f)));
	vertices.push_back(Vertex(glm::vec3(-1.0f, -1.0f,  1.0f)));
	vertices.push_back(Vertex(glm::vec3(1.0f, -1.0f,  1.0f)));

	m_mesh = new Mesh(vertices, mat);
	m_mesh->updateGL();

	addComponent(m_mesh);
}

Skybox::~Skybox()
{
	delete(m_mesh);
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