#pragma once

#include "Light.h"

class DirectionalLight : public Light
{
public:

	glm::vec3 direction;

	DirectionalLight();
	~DirectionalLight();

	void setLightUniform(Shader* shader);

private:

};

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