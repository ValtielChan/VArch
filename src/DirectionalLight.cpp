#include "DirectionalLight.h"

#include "Shader.h"

DirectionalLight::DirectionalLight() : Light(), direction(glm::vec3(0, -1, 0))
{
}

DirectionalLight::~DirectionalLight()
{
}

void DirectionalLight::setLightUniform(Shader* shader, int index)
{
	shader->setUniform3f("directionalLight.direction", direction);

	shader->setUniform3f("directionalLight.ambient", ambient);
	shader->setUniform3f("directionalLight.diffuse", diffuse);
	shader->setUniform3f("directionalLight.specular", specular);
}