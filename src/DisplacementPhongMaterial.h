#ifndef DISPLACEMENTPHONGMATERIAL_H
#define DISPLACEMENTPHONGMATERIAL_H

#include "Material.h"

class HeightMap;
class NormalMap;

class DisplacementPhongMaterial : public Material
{
public:

	// Parameter
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;

	// Textures
	HeightMap *heightmap;
	NormalMap *normalMap;

	DisplacementPhongMaterial();
	~DisplacementPhongMaterial();

	void setUniform();
};

#endif // DISPLACEMENTPHONGMATERIAL_H
