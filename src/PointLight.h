#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "Light.h"

#define LC "[PointLight] "

class PointLight : public Light
{
public:

	glm::vec3 position;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float constant;
	float linear;
	float quadratic;

	PointLight();
	~PointLight();

	void setLightUniform(Shader* shader, int index = 0);
};

#endif // POINTLIGHT_H