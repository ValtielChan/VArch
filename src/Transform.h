#pragma once

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Transform
{
	glm::vec3 m_position;
	glm::vec3 m_rotation;
	glm::vec3 m_scale;

public:

	Transform(glm::vec3 position = glm::vec3(0, 0, 0), glm::vec3 rotation = glm::vec3(0, 0, 0), glm::vec3 scale = glm::vec3(1, 1, 1))
		: m_position(position),
		m_rotation(rotation),
		m_scale(scale) {}

	~Transform() {}

	glm::vec3 position() { return m_position; }
	glm::vec3 rotation() { return m_rotation; }
	glm::vec3 scale() { return m_scale; }

	void setPosition(glm::vec3 position) { m_position = position; }
	void setRotation(glm::vec3 rotation) { m_rotation = rotation; }
	void setScale(glm::vec3 scale) { m_scale = scale; }

	void setPosX(float x) { m_position.x = x; }
	void setPosY(float y) { m_position.y = y; }
	void setPosZ(float z) { m_position.z = z; }

	void setRotX(float x) { m_rotation.x = x; }
	void setRotY(float y) { m_rotation.y = y; }
	void setRotZ(float z) { m_rotation.z = z; }

	void translate(glm::vec3 translation) { m_position += translation; }
	void rotate(glm::vec3 eulerAngle) { m_rotation += eulerAngle; }

	glm::mat4 model() {

		glm::mat4 model;
		model = glm::translate(model, m_position);

		model = glm::rotate(model, glm::radians(m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

		model = glm::scale(model, m_scale);

		return model;
	}
};

