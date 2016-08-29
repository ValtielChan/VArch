#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Singleton.h"
#include "Shaders.h"

class MVP : public Singleton<MVP> {

	friend class Singleton<MVP>;

private:

	MVP() {}
	~MVP() {}

	glm::mat4 m_model;
	glm::mat4 m_view;
	glm::mat4 m_projection;

public:

	glm::mat4 model() { return m_model; }
	glm::mat4 view() { return m_view; }
	glm::mat4 projection() { return m_projection; }

	void setModel(glm::mat4 model) { m_model = model; }
	void setView(glm::mat4 view) { m_view = view; }
	void setProjection(glm::mat4 projection) { m_projection = projection; }

	void setMVP(glm::mat4 model, glm::mat4 view, glm::mat4 projection) {
		m_model = model;
		m_view = view;
		m_projection = projection;
	}

	static void setUniform() {

		Shader *shader = Shaders::getInstance()->currentShader();
		MVP *mvp = MVP::getInstance();

		GLint modelLoc = glGetUniformLocation(shader->program, "model");
		GLint viewLoc = glGetUniformLocation(shader->program, "view");
		GLint projectionLoc = glGetUniformLocation(shader->program, "projection");

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mvp->model()));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(mvp->view()));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(mvp->projection()));
	}

	static void setUniform(Shader* shader) {

		MVP *mvp = MVP::getInstance();

		GLint modelLoc = glGetUniformLocation(shader->program, "model");
		GLint viewLoc = glGetUniformLocation(shader->program, "view");
		GLint projectionLoc = glGetUniformLocation(shader->program, "projection");

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mvp->model()));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(mvp->view()));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(mvp->projection()));
	}
};