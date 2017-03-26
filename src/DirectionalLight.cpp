#include "DirectionalLight.h"

#include "Light.h"
#include "Shader.h"

DirectionalLight::DirectionalLight() : Light(), direction(glm::vec3(0, -1, 0))
{
}

DirectionalLight::~DirectionalLight()
{
}

void DirectionalLight::setLightUniform(Shader* shader)
{
	shader->setUniform3f("light.direction", direction);

	shader->setUniform3f("light.ambient", ambient);
	shader->setUniform3f("light.diffuse", diffuse);
	shader->setUniform3f("light.specular", specular);


}