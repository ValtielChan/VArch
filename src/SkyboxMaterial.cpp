#include "SkyboxMaterial.h"



SkyboxMaterial::SkyboxMaterial()
	: Material(Shaders::getInstance()->getShader(BuiltInShader::SKYBOX))
{
}


SkyboxMaterial::~SkyboxMaterial()
{
}

void SkyboxMaterial::setUniform()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
	m_shader->setUniform1i("skybox", 0);
}
