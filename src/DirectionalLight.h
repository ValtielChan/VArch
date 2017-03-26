#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include "Light.h"

class DirectionalLight : public Light
{
public:

	glm::vec3 direction;

	DirectionalLight();
	~DirectionalLight();

	void setLightUniform(Shader* shader);

private:

};

#endif