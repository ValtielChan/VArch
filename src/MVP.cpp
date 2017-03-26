#include "MVP.h"

#include "Shaders.h"

void MVP::setMVP(glm::mat4 model, glm::mat4 view, glm::mat4 projection) {
	m_model = model;
	m_view = view;
	m_projection = projection;
}

void MVP::setUniform() {

	Shader *shader = Shaders::getInstance()->currentShader();
	MVP *mvp = MVP::getInstance();

	GLint modelLoc = glGetUniformLocation(shader->program, "model");
	GLint viewLoc = glGetUniformLocation(shader->program, "view");
	GLint projectionLoc = glGetUniformLocation(shader->program, "projection");

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mvp->model()));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(mvp->view()));
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(mvp->projection()));
}

void MVP::setUniform(Shader* shader) {

	MVP *mvp = MVP::getInstance();

	GLint modelLoc = glGetUniformLocation(shader->program, "model");
	GLint viewLoc = glGetUniformLocation(shader->program, "view");
	GLint projectionLoc = glGetUniformLocation(shader->program, "projection");

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mvp->model()));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(mvp->view()));
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(mvp->projection()));
}
