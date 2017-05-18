#pragma once

#include "Material.h"

class SkyboxMaterial : public Material
{
public:
	GLuint cubemap;

	SkyboxMaterial();
	~SkyboxMaterial();

	void setUniform();
};

