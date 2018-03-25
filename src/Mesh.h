#ifndef MESH_H
#define MESH_H

#define GLEW_STATIC
#include <GL/glew.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <algorithm>
#include <vector>

#include "enum.h"
#include "Component.h"

class Material;
class Shader;

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
	Mesh(std::vector<Vertex> vertices, Material *material);
	Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, Material *material);
	~Mesh();

	std::vector<Vertex>* getVertices() { return &m_vertices; }
	std::vector<GLuint>* getIndices() { return &m_indices; }

	void update();
	void update(glm::mat4 model, Shader* shader = NULL);

	void setMaterial(Material* material) { m_material = material; }

	void draw();
	void updateGL();
	void generateCube(float size = 1.0f, glm::vec3 offset = glm::vec3(0, 0, 0), glm::vec3 color = glm::vec3(0.5, 0.5, 0.5));
	void generateLandmark();

	glm::vec3 getMax();
	glm::vec3 getMin();

	void normalizePositions(glm::vec3 norm);
	void projectToSphere(Side side);
	void translate(glm::vec3 translation);
	void scale(glm::vec3 scale);
	void rotate(glm::vec3 rotation);

	void exportObj(const char* filename);

	int sizeOf();
};

#endif // MESH_H