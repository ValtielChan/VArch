#pragma once

#include "Material.h"

class PhongMaterial : public Material
{
public:

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;

	PhongMaterial(glm::vec3 a = glm::vec3(0.15), glm::vec3 d = glm::vec3(0.5), glm::vec3 s = glm::vec3(0.5), float sh = 16);
	~PhongMaterial();

	void setUniform();
};

// Default = Gold
PhongMaterial::PhongMaterial(glm::vec3 a, glm::vec3 d, glm::vec3 s, float sh)
	: Material(Shaders::getInstance()->getShader(BuiltInShader::PHONG)),
	ambient(a),
	diffuse(d),
	specular(d),
	shininess(sh)
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

namespace BuiltInMaterial {

	/*const PhongMaterial* Gold = new PhongMaterial(
		glm::vec3(0.24725f, 0.1995f, 0.0745f),
		glm::vec3(0.75164f, 0.60648f, 0.22648f),
		glm::vec3(0.628281f, 0.555802f, 0.366065f),
		32.0f
		);*/
}