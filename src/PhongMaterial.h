#pragma once

#include "Material.h"

class PhongMaterial : public Material
{
public:

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;

	PhongMaterial();
	~PhongMaterial();

	void setUniform();
};

// Default = Gold
PhongMaterial::PhongMaterial()
	: Material(Shaders::getInstance()->getShader(BuiltInShader::PHONG)),
	ambient(glm::vec3(0.24725f, 0.1995f, 0.0745f)),
	diffuse(glm::vec3(0.75164f, 0.60648f, 0.22648f)),
	specular(glm::vec3(0.628281f, 0.555802f, 0.366065f)),
	shininess(32.0f)
{
}

PhongMaterial::~PhongMaterial()
{
}

void PhongMaterial::setUniform()
{
	m_shader->setUniform3f("material.ambient", ambient);
	m_shader->setUniform3f("material.diffuse", diffuse);
	m_shader->setUniform3f("material.specular", specular);
	m_shader->setUniform1f("material.shininess", shininess);
}
