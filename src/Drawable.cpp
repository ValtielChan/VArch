#include "Drawable.h"

#include "Shaders.h"
#include "MVP.h"

void Drawable::setMVPUniform()
{
	Shader *shader = Shaders::getInstance()->currentShader();
	MVP *mvp = MVP::getInstance();

	GLint modelLoc = glGetUniformLocation(shader->program, "model");
	GLint viewLoc = glGetUniformLocation(shader->program, "view");
	GLint projectionLoc = glGetUniformLocation(shader->program, "projection");

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mvp->model()));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(mvp->view()));
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(mvp->projection()));
}