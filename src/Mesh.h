#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Component.h"
#include "PhongMaterial.h" // Choosen as Default Mesh Material
#include "MVP.h"

struct Vertex {

	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;
	glm::vec3 color;

	Vertex() {}
	Vertex(glm::vec3 p = glm::vec3(0), glm::vec3 n = glm::vec3(0, 1, 0), glm::vec2 t = glm::vec2(0), glm::vec3 c = glm::vec3(1.f))
		: position(p), normal(n), color(c), texCoord(t) {}
};

class Mesh : public Component
{
private:

	Material* m_material;

	std::vector<Vertex> m_vertices;
	std::vector<GLuint> m_indices;

	GLuint m_VAO, m_VBO, m_EBO;

public:

	Mesh();
	Mesh(Material *material);
	Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, Material *material);
	~Mesh();

	std::vector<Vertex>* getVertices() { return &m_vertices; }
	std::vector<GLuint>* getIndices() { return &m_indices; }

	void update();
	void update(glm::mat4 model);

	void setMaterial(Material* material) { m_material = material; }

	void draw();
	void updateGL();
	void generateCube();
};

Mesh::Mesh()
{
	m_material = new PhongMaterial();

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	updateGL();
}

Mesh::Mesh(Material *material) : m_material(material)
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	generateCube();
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, Material *material)
	: m_vertices(vertices),
	m_indices(indices),
	m_material(material)
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_EBO);
}

void Mesh::update()
{
	updateGL();
	//m_material->setUniform();
	draw();
}

void Mesh::update(glm::mat4 model)
{

	Shader* shader = m_material->getShader();
	shader->use();

	MVP::getInstance()->setModel(model);
	MVP::setUniform(shader);

	updateGL();
	m_material->setUniform();
	draw();
}

void Mesh::draw()
{
	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::updateGL()
{
	if (m_vertices.size() > 0 && m_indices.size() > 0) {

		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

		glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex),
			&m_vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint),
			&m_indices[0], GL_STATIC_DRAW);

		// Vertex Positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
			(GLvoid*)0);

		// Vertex Normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
			(GLvoid*)offsetof(Vertex, normal));

		// Vertex Texture Coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
			(GLvoid*)offsetof(Vertex, texCoord));

		// Vertex Color
		/*glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(GLvoid*)offsetof(Vertex, color));*/

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

	}
}

void Mesh::generateCube()
{
	m_vertices.push_back(Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f)));
	m_vertices.push_back(Vertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f)));
	m_vertices.push_back(Vertex(glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f)));
	m_vertices.push_back(Vertex(glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f)));

	m_vertices.push_back(Vertex(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f)));
	m_vertices.push_back(Vertex(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f)));
	m_vertices.push_back(Vertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f)));
	m_vertices.push_back(Vertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f)));

	m_vertices.push_back(Vertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(-1.0f, 0.0f, 0.0f)));
	m_vertices.push_back(Vertex(glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(-1.0f, 0.0f, 0.0f)));
	m_vertices.push_back(Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-1.0f, 0.0f, 0.0f)));
	m_vertices.push_back(Vertex(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(-1.0f, 0.0f, 0.0f)));

	m_vertices.push_back(Vertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 0.0f, 0.0f)));
	m_vertices.push_back(Vertex(glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 0.0f)));
	m_vertices.push_back(Vertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 0.0f)));
	m_vertices.push_back(Vertex(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(1.0f, 0.0f, 0.0f)));

	m_vertices.push_back(Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f)));
	m_vertices.push_back(Vertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f)));
	m_vertices.push_back(Vertex(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.0f, -1.0f, 0.0f)));
	m_vertices.push_back(Vertex(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.0f, -1.0f, 0.0f)));

	m_vertices.push_back(Vertex(glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f)));
	m_vertices.push_back(Vertex(glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f)));
	m_vertices.push_back(Vertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 1.0f, 0.0f)));
	m_vertices.push_back(Vertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 1.0f, 0.0f)));

	m_indices.push_back(0); m_indices.push_back(1); m_indices.push_back(2);
	m_indices.push_back(3); m_indices.push_back(0); m_indices.push_back(2);

	m_indices.push_back(4); m_indices.push_back(5); m_indices.push_back(6);
	m_indices.push_back(7); m_indices.push_back(4); m_indices.push_back(6);

	m_indices.push_back(8); m_indices.push_back(9); m_indices.push_back(10);
	m_indices.push_back(11); m_indices.push_back(8); m_indices.push_back(10);

	m_indices.push_back(12); m_indices.push_back(13); m_indices.push_back(14);
	m_indices.push_back(15); m_indices.push_back(12); m_indices.push_back(14);

	m_indices.push_back(16); m_indices.push_back(17); m_indices.push_back(18);
	m_indices.push_back(19); m_indices.push_back(16); m_indices.push_back(18);

	m_indices.push_back(20); m_indices.push_back(21); m_indices.push_back(22);
	m_indices.push_back(23); m_indices.push_back(20); m_indices.push_back(22);

	updateGL();
}
