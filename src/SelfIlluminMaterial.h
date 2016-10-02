#pragma once

#include "Material.h"

class SelfIlluminMaterial : public Material
{
public:

	SelfIlluminMaterial();
	~SelfIlluminMaterial();

	void setUniform();
};

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
