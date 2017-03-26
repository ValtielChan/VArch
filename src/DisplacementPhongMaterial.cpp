#include "DisplacementPhongMaterial.h"

#include "Material.h"
#include "HeightMap.h"
#include "NormalMap.h"

// Default = Gold
DisplacementPhongMaterial::DisplacementPhongMaterial()

	: Material(Shaders::getInstance()->getShader(BuiltInShader::DISPLACEMENT)),
	ambient(glm::vec3(0.24725f, 0.1995f, 0.0745f)),
	diffuse(glm::vec3(0.75164f, 0.60648f, 0.22648f)),
	specular(glm::vec3(0.628281f, 0.555802f, 0.366065f)),
	shininess(32.0f)
{
}

DisplacementPhongMaterial::~DisplacementPhongMaterial()
{
}

void DisplacementPhongMaterial::setUniform()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, heightmap->texture());
	m_shader->setUniform1i("heightMap", 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, normalMap->texture());
	m_shader->setUniform1i("normalMap", 1);

	m_shader->setUniform3f("material.ambient", ambient);
	m_shader->setUniform3f("material.diffuse", diffuse);
	m_shader->setUniform3f("material.specular", specular);
	m_shader->setUniform1f("material.shininess", shininess);
}
