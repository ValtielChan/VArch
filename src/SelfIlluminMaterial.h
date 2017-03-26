#ifndef SELFILLUMINMATERIAL_H
#define SELFILLUMINMATERIAL_H

#include "Material.h"

class SelfIlluminMaterial : public Material
{
public:

	SelfIlluminMaterial();
	~SelfIlluminMaterial();

	void setUniform();
};

#endif // !SELFILLUMINMATERIAL_H