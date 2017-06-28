#include "PointLight.h"
#include "Shader.h"

#include <string>

PointLight::PointLight() : Light(), constant(1.f), linear(0.09f), quadratic(0.032f)
{
}


PointLight::~PointLight()
{
}

void PointLight::setLightUniform(Shader * shader, int index)
{
	shader->setUniform3f(("pointLights[" + std::to_string(index) + "].position").c_str(), position);

	shader->setUniform3f(("pointLights[" + std::to_string(index) + "].ambient").c_str(), ambient);
	shader->setUniform3f(("pointLights[" + std::to_string(index) + "].diffuse").c_str(), diffuse);
	shader->setUniform3f(("pointLights[" + std::to_string(index) + "].specular").c_str(), specular);

	shader->setUniform1f(("pointLights[" + std::to_string(index) + "].constant").c_str(), constant);
	shader->setUniform1f(("pointLights[" + std::to_string(index) + "].linear").c_str(), linear);
	shader->setUniform1f(("pointLights[" + std::to_string(index) + "].quadratic").c_str(), quadratic);

}
