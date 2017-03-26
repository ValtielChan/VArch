#include "SelfIlluminMaterial.h"

// Default = Gold
SelfIlluminMaterial::SelfIlluminMaterial()
	: Material(Shaders::getInstance()->getShader(BuiltInShader::SELFILLUMIN))
{
}

SelfIlluminMaterial::~SelfIlluminMaterial()
{
}

void SelfIlluminMaterial::setUniform()
{

}
