#include "Mesh.h"

#include "Component.h"
#include "PhongMaterial.h" // Choosen as Default Mesh Material
#include "MVP.h"

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

	updateGL();
	//generateCube(1.f, glm::vec3(1, 2, 3), glm::vec3(.5f));
}

Mesh::Mesh(std::vector<Vertex> vertices, Material *material)
	: m_vertices(vertices),
	m_material(material)
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
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

void Mesh::update(glm::mat4 model, Shader * shader)
{
	if (shader == NULL)
		shader = m_material->getShader();

	shader->use();

	MVP::getInstance()->setModel(model);
	MVP::setUniform(shader);

	//updateGL();
	m_material->setUniform();
	draw();
}

void Mesh::draw()
{
	glBindVertexArray(m_VAO);

	if (m_indices.size() > 0) // if EBO is set
		glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
	else
		glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());

	glBindVertexArray(0);
}

void Mesh::updateGL()
{
	if (m_vertices.size() > 0) {

		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

		glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex),
			&m_vertices[0], GL_STATIC_DRAW);

		if (m_indices.size() > 0) {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint),
				&m_indices[0], GL_STATIC_DRAW);
		}

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
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(GLvoid*)offsetof(Vertex, color));

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

	}
}

void Mesh::generateCube(float size, glm::vec3 offset, glm::vec3 color)
{
	float halfsize = size / 2.f;

	int vaoSize = m_vertices.size();

	m_vertices.push_back(Vertex(glm::vec3(-halfsize, -halfsize, -halfsize) + offset, glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0), color));
	m_vertices.push_back(Vertex(glm::vec3(halfsize, -halfsize, -halfsize) + offset, glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0), color));
	m_vertices.push_back(Vertex(glm::vec3(halfsize, halfsize, -halfsize) + offset, glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0), color));
	m_vertices.push_back(Vertex(glm::vec3(-halfsize, halfsize, -halfsize) + offset, glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0), color));

	m_vertices.push_back(Vertex(glm::vec3(-halfsize, -halfsize, halfsize) + offset, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0), color));
	m_vertices.push_back(Vertex(glm::vec3(halfsize, -halfsize, halfsize) + offset, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0), color));
	m_vertices.push_back(Vertex(glm::vec3(halfsize, halfsize, halfsize) + offset, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0), color));
	m_vertices.push_back(Vertex(glm::vec3(-halfsize, halfsize, halfsize) + offset, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0), color));

	m_vertices.push_back(Vertex(glm::vec3(-halfsize, halfsize, halfsize) + offset, glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0), color));
	m_vertices.push_back(Vertex(glm::vec3(-halfsize, halfsize, -halfsize) + offset, glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0), color));
	m_vertices.push_back(Vertex(glm::vec3(-halfsize, -halfsize, -halfsize) + offset, glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0), color));
	m_vertices.push_back(Vertex(glm::vec3(-halfsize, -halfsize, halfsize) + offset, glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0), color));

	m_vertices.push_back(Vertex(glm::vec3(halfsize, halfsize, halfsize) + offset, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0), color));
	m_vertices.push_back(Vertex(glm::vec3(halfsize, halfsize, -halfsize) + offset, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0), color));
	m_vertices.push_back(Vertex(glm::vec3(halfsize, -halfsize, -halfsize) + offset, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0), color));
	m_vertices.push_back(Vertex(glm::vec3(halfsize, -halfsize, halfsize) + offset, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0), color));
	
	m_vertices.push_back(Vertex(glm::vec3(-halfsize, -halfsize, -halfsize) + offset, glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0), color));
	m_vertices.push_back(Vertex(glm::vec3(halfsize, -halfsize, -halfsize) + offset, glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0), color));
	m_vertices.push_back(Vertex(glm::vec3(halfsize, -halfsize, halfsize) + offset, glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0), color));
	m_vertices.push_back(Vertex(glm::vec3(-halfsize, -halfsize, halfsize) + offset, glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0), color));

	m_vertices.push_back(Vertex(glm::vec3(-halfsize, halfsize, -halfsize) + offset, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0), color));
	m_vertices.push_back(Vertex(glm::vec3(halfsize, halfsize, -halfsize) + offset, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0), color));
	m_vertices.push_back(Vertex(glm::vec3(halfsize, halfsize, halfsize) + offset, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0), color));
	m_vertices.push_back(Vertex(glm::vec3(-halfsize, halfsize, halfsize) + offset, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0), color));

	m_indices.push_back(vaoSize + 0); m_indices.push_back(vaoSize + 1); m_indices.push_back(vaoSize + 2);
	m_indices.push_back(vaoSize + 3); m_indices.push_back(vaoSize + 0); m_indices.push_back(vaoSize + 2);

	m_indices.push_back(vaoSize + 4); m_indices.push_back(vaoSize + 5); m_indices.push_back(vaoSize + 6);
	m_indices.push_back(vaoSize + 7); m_indices.push_back(vaoSize + 4); m_indices.push_back(vaoSize + 6);

	m_indices.push_back(vaoSize + 8); m_indices.push_back(vaoSize + 9); m_indices.push_back(vaoSize + 10);
	m_indices.push_back(vaoSize + 11); m_indices.push_back(vaoSize + 8); m_indices.push_back(vaoSize + 10);

	m_indices.push_back(vaoSize + 12); m_indices.push_back(vaoSize + 13); m_indices.push_back(vaoSize + 14);
	m_indices.push_back(vaoSize + 15); m_indices.push_back(vaoSize + 12); m_indices.push_back(vaoSize + 14);

	m_indices.push_back(vaoSize + 16); m_indices.push_back(vaoSize + 17); m_indices.push_back(vaoSize + 18);
	m_indices.push_back(vaoSize + 19); m_indices.push_back(vaoSize + 16); m_indices.push_back(vaoSize + 18);

	m_indices.push_back(vaoSize + 20); m_indices.push_back(vaoSize + 21); m_indices.push_back(vaoSize + 22);
	m_indices.push_back(vaoSize + 23); m_indices.push_back(vaoSize + 20); m_indices.push_back(vaoSize + 22);

	updateGL();
}

void Mesh::generateLandmark()
{
	generateCube(.2f, glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));

	generateCube(.2f, glm::vec3(0.5f, 0, 0), glm::vec3(1, 0, 0));
	generateCube(.2f, glm::vec3(0, 0.5f, 0), glm::vec3(0, 1, 0));
	generateCube(.2f, glm::vec3(0, 0, 0.5f), glm::vec3(0, 0, 1));
}

glm::vec3 Mesh::getMax()
{
	glm::vec3 max = m_vertices[0].position;

	for (Vertex v : m_vertices) {
		max.x = std::max(max.x, v.position.x);
		max.y = std::max(max.y, v.position.y);
		max.z = std::max(max.z, v.position.z);
	}

	return max;
}

glm::vec3 Mesh::getMin()
{
	glm::vec3 min = m_vertices[0].position;

	for (Vertex v : m_vertices) {
		min.x = std::min(min.x, v.position.x);
		min.y = std::min(min.y, v.position.y);
		min.z = std::min(min.z, v.position.z);
	}

	return min;
}

void Mesh::normalizePositions(glm::vec3 norm) {

	for (auto it = m_vertices.begin(); it != m_vertices.end(); it++) {
		(*it).position *= norm;
	}

	updateGL();
}

void Mesh::projectToSphere(Side side)
{
	for (auto it = m_vertices.begin(); it != m_vertices.end(); it++) {

		float x2 = (*it).position.x * (*it).position.x;
		float y2 = (*it).position.y * (*it).position.y;
		float z2 = (*it).position.z * (*it).position.z;

		float x = (*it).position.x;
		float y = (*it).position.y;
		float z = (*it).position.z;

		float w = 0.f;

		if (side == Side::UP) {
			w = sqrt(x2 + 1 + z2);

			(*it).position.x = y * (x / w);
			(*it).position.y = y * (1 / w);
			(*it).position.z = y * (z / w);

			float w2 = w*w;
			float w3 = w*w*w;

			glm::mat3 J(
				glm::vec3(y / (w*(1 - x2 / (w2))), (-x*y) / (w3), (-x*z*y) / (w3)),
				glm::vec3((-x*z*y) / (w3), (-z*y) / (w3), y / (w*(1 - z2 / (w2)))),
				glm::vec3(x / w, 1 / w, z / w)
			);

			glm::vec3 T = J * glm::vec3(1.f, 0.f, 0.f);
			glm::vec3 B = J * glm::vec3(0.f, 0.f, 1.f);
			glm::vec3 N = glm::cross(T, B);

			glm::mat3 TBN(T, B, N);

			if ((int)glm::dot((*it).normal, glm::vec3(0.f, 1.f, 0.f)) == 1.f)
				(*it).normal = glm::normalize((*it).position);
			else
				(*it).normal = TBN * (*it).normal;


		}
		else if (side == Side::DOWN) {
			w = sqrt(x2 + 1 + z2);

			(*it).position.x = -y * (x / w);
			(*it).position.y = y * (1 / w);
			(*it).position.z = -y * (z / w);

			if ((int)glm::dot((*it).normal, glm::vec3(0.f, -1.f, 0.f)) == 1.f) {
				(*it).normal = glm::normalize((*it).position);
			}
			else if ((int)glm::dot((*it).normal, glm::vec3(1.f, 0.f, 0.f)) == 1.f) {

				glm::vec3 N = glm::normalize((*it).position);
				glm::vec3 T = glm::cross(N, glm::vec3(-1.f, 0.f, 0.f));
				(*it).normal = (glm::cross(N, T));
			}
			else if ((int)glm::dot((*it).normal, glm::vec3(-1.f, 0.f, 0.f)) == 1.f) {

				glm::vec3 N = glm::normalize((*it).position);
				glm::vec3 T = glm::cross(N, glm::vec3(1.f, 0.f, 0.f));
				(*it).normal = (glm::cross(N, T));
			}
			else if ((int)glm::dot((*it).normal, glm::vec3(0.f, 0.f, 1.f)) == 1.f) {

				glm::vec3 N = glm::normalize((*it).position);
				glm::vec3 T = glm::cross(N, glm::vec3(0.f, 0.f, -1.f));
				(*it).normal = (glm::cross(N, T));
			}
			else if ((int)glm::dot((*it).normal, glm::vec3(0.f, 0.f, -1.f)) == 1.f) {

				glm::vec3 N = glm::normalize((*it).position);
				glm::vec3 T = glm::cross(N, glm::vec3(0.f, 0.f, 1.f));
				(*it).normal = (glm::cross(N, T));
			}

		}
		else if (side == Side::FRONT) {
			w = sqrt(1 + y2 + z2);

			(*it).position.x = x * (1 / w);
			(*it).position.y = x * (y / w);
			(*it).position.z = x * (z / w);

			float w2 = w*w;
			float w3 = w*w*w;

			glm::mat3 J(
				glm::vec3((-z*x) / (w3), (-z*y*x) / (w3), x / (w*(1 - z2 / (w2)))),
				glm::vec3((-y*x) / (w3), x / (w*(1 - y2 / (w2))), (-z*y*x) / (w3)),
				glm::vec3(1 / w, y / w, z / w)
			);

			glm::vec3 T = J * glm::vec3(0.f, 0.f, 1.f);
			glm::vec3 B = J * glm::vec3(0.f, 1.f, 0.f);
			glm::vec3 N = glm::cross(T, B);

			glm::mat3 TBN(T, B, N);

			if ((int)glm::dot((*it).normal, glm::vec3(1.f, 0.f, 0.f)) == 1.f)
				(*it).normal = glm::normalize((*it).position);
			else
				(*it).normal = TBN * (*it).normal;

		}
		else if (side == Side::BACK) {
			w = sqrt(1 + y2 + z2);

			(*it).position.x = x * (1 / w);
			(*it).position.y = -x * (y / w);
			(*it).position.z = -x * (z / w);

			if ((int)glm::dot((*it).normal, glm::vec3(-1.f, 0.f, 0.f)) == 1.f) {
				(*it).normal = glm::normalize((*it).position);
			}
			else if ((int)glm::dot((*it).normal, glm::vec3(0.f, 1.f, 0.f)) == 1.f) {

				glm::vec3 N = glm::normalize((*it).position);
				glm::vec3 T = glm::cross(N, glm::vec3(0.f, -1.f, 0.f));
				(*it).normal = (glm::cross(N, T));
			}
			else if ((int)glm::dot((*it).normal, glm::vec3(0.f, -1.f, 0.f)) == 1.f) {

				glm::vec3 N = glm::normalize((*it).position);
				glm::vec3 T = glm::cross(N, glm::vec3(0.f, 1.f, 0.f));
				(*it).normal = (glm::cross(N, T));
			}
			else if ((int)glm::dot((*it).normal, glm::vec3(0.f, 0.f, 1.f)) == 1.f) {

				glm::vec3 N = glm::normalize((*it).position);
				glm::vec3 T = glm::cross(N, glm::vec3(0.f, 0.f, -1.f));
				(*it).normal = (glm::cross(N, T));
			}
			else if ((int)glm::dot((*it).normal, glm::vec3(0.f, 0.f, -1.f)) == 1.f) {

				glm::vec3 N = glm::normalize((*it).position);
				glm::vec3 T = glm::cross(N, glm::vec3(0.f, 0.f, 1.f));
				(*it).normal = (glm::cross(N, T));
			}

		}
		else if (side == Side::RIGHT) {
			w = sqrt(x2 + y2 + 1);

			(*it).position.x = z * (x / w);
			(*it).position.y = z * (y / w);
			(*it).position.z = z * (1 / w);

			float w2 = w*w;
			float w3 = w*w*w;

			glm::mat3 J(
				glm::vec3(z / (w*(1 - x2 / (w2))), (-x*y*z) / (w3), (-x*z) / (w3)),
				glm::vec3((-x*y*z) / (w3), z / (w*(1 - y2 / (w2))), (-y*z) / (w3)),
				glm::vec3(x / w, y / w, 1 / w)
			);

			glm::vec3 T = J * glm::vec3(1.f, 0.f, 0.f);
			glm::vec3 B = J * glm::vec3(0.f, 1.f, 0.f);
			glm::vec3 N = glm::cross(T, B);

			glm::mat3 TBN(T, B, N);

			if ((int)glm::dot((*it).normal, glm::vec3(0.f, 0.f, 1.f)) == 1.f)
				(*it).normal = glm::normalize((*it).position);
			else
				(*it).normal = TBN * (*it).normal;

		}
		else if (side == Side::LEFT) {
			w = sqrt(x2 + y2 + 1);

			(*it).position.x = -z * (x / w);
			(*it).position.y = -z * (y / w);
			(*it).position.z = z * (1 / w);

			if ((int)glm::dot((*it).normal, glm::vec3(0.f, 0.f, -1.f)) == 1.f) {
				(*it).normal = glm::normalize((*it).position);
			}
			else if ((int)glm::dot((*it).normal, glm::vec3(0.f, 1.f, 0.f)) == 1.f) {

				glm::vec3 N = glm::normalize((*it).position);
				glm::vec3 T = glm::cross(N, glm::vec3(0.f, -1.f, 0.f));
				(*it).normal = (glm::cross(N, T));
			}
			else if ((int)glm::dot((*it).normal, glm::vec3(0.f, -1.f, 0.f)) == 1.f) {

				glm::vec3 N = glm::normalize((*it).position);
				glm::vec3 T = glm::cross(N, glm::vec3(0.f, 1.f, 0.f));
				(*it).normal = (glm::cross(N, T));
			}
			else if ((int)glm::dot((*it).normal, glm::vec3(1.f, 0.f, 0.f)) == 1.f) {

				glm::vec3 N = glm::normalize((*it).position);
				glm::vec3 T = glm::cross(N, glm::vec3(-1.f, 0.f, 0.f));
				(*it).normal = (glm::cross(N, T));
			}
			else if ((int)glm::dot((*it).normal, glm::vec3(-1.f, 0.f, 0.f)) == 1.f) {

				glm::vec3 N = glm::normalize((*it).position);
				glm::vec3 T = glm::cross(N, glm::vec3(1.f, 0.f, 0.f));
				(*it).normal = (glm::cross(N, T));
			}
		}
	}

	updateGL();
}

void Mesh::translate(glm::vec3 translation)
{
	for (auto it = m_vertices.begin(); it != m_vertices.end(); it++) {
		(*it).position = (*it).position + translation;
	}

	updateGL();
}

void Mesh::scale(glm::vec3 scale)
{
	for (auto it = m_vertices.begin(); it != m_vertices.end(); it++) {
		(*it).position = (*it).position * scale;
	}

	updateGL();
}

void Mesh::rotate(glm::vec3 rotation)
{
	glm::mat4 model = glm::mat4();

	model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.f, 0.f, 0.f));
	model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f));
	model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f));
	model = glm::scale(model, glm::vec3(1.f));

	for (auto it = m_vertices.begin(); it != m_vertices.end(); it++) {
		(*it).position = glm::vec3( glm::vec4((*it).position, 1.f) * model);
		(*it).normal = (*it).normal * glm::transpose(glm::inverse(glm::mat3(model)));
	}

	updateGL();
}



int Mesh::sizeOf()
{
	return m_indices.size() * sizeof(int) + m_vertices.size() * sizeof(Vertex) + sizeof(*this);
}
