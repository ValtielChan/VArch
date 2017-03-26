#include "PhongMaterial.h"

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